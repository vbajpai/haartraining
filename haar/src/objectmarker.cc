/* opencv library */
#include<cv.h>
#include<highgui.h>

/* posix library */
#include<dirent.h>

/* c++ standard library */
#include<iostream>
#include<fstream>
using namespace std;

/* global constants */
bool isFirstClick = true;
int roiX0, roiX1, roiY0, roiY1;
IplImage* image;
string WINDOW_TITLE = "ADD:<SPACE> | SAVE&LOADNEXT:<ENTER> | EXIT:<ESC>";

void mouseCallback(int event, int x, int y, int flags, void* param){
	
	if(event==CV_EVENT_LBUTTONDOWN)
	{
		if(isFirstClick == true)
		{
			roiX0=x;
			roiY0=y;
			isFirstClick = false;				
		} else {
			
			roiX1=x;
			roiY1=y;
			isFirstClick = true;
		}
	}
	if(event==CV_EVENT_MOUSEMOVE && isFirstClick == false)
	{
		
		/* draw the rectangle on ROI */
		IplImage* rectImage = cvCloneImage(image);
		cvRectangle(rectImage, cvPoint(roiX0,roiY0), cvPoint(x,y), CV_RGB(0,255,0), 1);
		cvShowImage(WINDOW_TITLE.c_str(),rectImage);
		cvReleaseImage(&rectImage);
	}
	
}

class ObjectMarker{
	
private:	

	/* constants */
	static const int KEY_ENTER = 13;
	static const int KEY_SPACE = 32;
	static const int KEY_ESCAPE = 27;	

	
	/* variables */
	string imagesDir;
	string outFileName;
	
protected:
	
	string preparePath(string fileName){
		string path;
		
		/* add a slash at the end of directory if not supplied */
		if (*(imagesDir.rbegin()) != '/')			
			imagesDir.append("/");	
		
		path = imagesDir + fileName;
		return path;
	}	
	
	string toString(int x){
		
		ostringstream intStream;
		intStream << x << flush;
		return(intStream.str());
	}

	
public:
	
	ObjectMarker(string imagesDir, string outFileName){
		this->imagesDir = imagesDir;
		this->outFileName = outFileName;		
	}
	
	void markObjects(){
		
		/* create a posix directory structure */
		DIR* dir = opendir(imagesDir.c_str());
		if(dir != NULL){
			
			struct dirent* dirEntry;
			ofstream outputFile(outFileName.c_str());
			
			cvNamedWindow(WINDOW_TITLE.c_str(), CV_WINDOW_AUTOSIZE);
			cvSetMouseCallback(WINDOW_TITLE.c_str(), mouseCallback, NULL);
			
			/* iterate the directory listing */
			while((dirEntry = readdir(dir)) != NULL){
				
				string filename = dirEntry->d_name;				
				if(filename != "." && filename != ".."){
					
					/* load the image */	
					string path = preparePath(filename);
					image = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_UNCHANGED);
					
					if(image != NULL){
						
						int keyPress = 0;
						int numberOfCarsInImage = 0; 
						string carParameters;
						cout << "loaded - " << filename << endl;
						
												
						do{
							/* show the image */
							cvShowImage(WINDOW_TITLE.c_str(), image);
							string width, height, pilotX, pilotY;
							
							keyPress = cvWaitKey(0);
							switch(keyPress){
									
								case KEY_ENTER:	
								case KEY_SPACE:
									
									numberOfCarsInImage += 1;
									
									if(roiX0 < roiX1 && roiY0 < roiY1){
										
										width = toString(roiX1 - roiX0);
										height = toString(roiY1 - roiY0);
										pilotX = toString(roiX0);
										pilotY = toString(roiY0);										
									}
									
									if(roiX0 > roiX1 && roiY0 > roiY1){
										
										width = toString(roiX0 - roiX1);
										height = toString(roiY0 - roiY1);
										pilotX = toString(roiX1);
										pilotY = toString(roiY1);										
									}
									
									carParameters += " " + pilotX + " " + pilotY + " " + width + " " + height;
									
									break;
									
								case KEY_ESCAPE:
									
									cvReleaseImage(&image);
									cvDestroyWindow(WINDOW_TITLE.c_str());
									return;
							}
							
							
						}while(keyPress != KEY_ENTER);
						
						if(numberOfCarsInImage != 0)
							outputFile << path << " " << numberOfCarsInImage << carParameters << "\n";						
						
						/* release the image */
						cvReleaseImage(&image);
					}else{
						cout << "Cannot Load Image" << endl;										
						cout << "Path: " << path << endl;
					}				
				}							
			}
			
			/* release resources */
			cvDestroyWindow(WINDOW_TITLE.c_str());
			outputFile.close();
			
		}else{
			
			cout << "Cannot Open Directory" << endl << endl;		
			return;
		}
	}

};

int main(int argc, char* argv[]){
	
	if(argc != 3) 		
		cout << "usage: objectmarker <imagesdirectory> <outputfile>" << endl;
	else {			
			
		ObjectMarker *marker = new ObjectMarker(argv[1], argv[2]);
		marker->markObjects();
		delete marker;			
	} 		
}