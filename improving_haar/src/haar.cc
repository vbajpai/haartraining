
/* opencv headers */
#include<cv.h>
#include<highgui.h>

/* posix headers */
#include<dirent.h>

/* c++ headers */
#include<iostream>
using namespace std;

class HaarImproved{
	
private:
	
	/* variables */
	CvHaarClassifierCascade* cascade;
	
	/* constants */
	static const string WINDOW_TITLE;
	static const string ANOTHER_WINDOW_TITLE;
	
	static const int HAAR_SCALE = 1;
	static const int HAAR_SCALE_FACTOR = 2;
	static const int HAAR_MIN_NEIGHBORS = 2;
	
	static const int RECTANGLE_THICKNESS = 3;
	static const int RECTANGLE_LINETYPE = 8;
	static const int RECTANGLE_SHIFT = 0;
	
	static const int SHADOW_THRESH = 20;
	
protected:
	
	IplImage* detectOnlyCars(IplImage* img){
		
		/* points to represent car' location */
		CvPoint point1, point2;
		
		/* detect objects */
		CvMemStorage* storage = cvCreateMemStorage(0);		
		CvSeq* cars = cvHaarDetectObjects(img, cascade, storage, HAAR_SCALE_FACTOR, HAAR_MIN_NEIGHBORS,
										  CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40));
		
		/* iterate the number of cars found */
		for(int i = 0; i < (cars ? cars->total : 0); i++){
			
			/* allocate a rectangle for drawing a car */
			CvRect* rectangle = (CvRect*)cvGetSeqElem(cars, i);
			
			/* find dimensions of the car (scale if necessary) */
			point1.x = rectangle->x*HAAR_SCALE;
			point2.x = (rectangle->x+rectangle->width)*HAAR_SCALE;
			point1.y = rectangle->y*HAAR_SCALE;
			point2.y = (rectangle->y+rectangle->height)*HAAR_SCALE;
			
			if(isBehindOfCar(point1, point2, img)){
				
				/* draw the rectangle on the image */
				cvRectangle(img, point1, point2, CV_RGB(255,0,0), RECTANGLE_THICKNESS, RECTANGLE_LINETYPE, RECTANGLE_SHIFT);				
			}
		}
		
		cvReleaseMemStorage(&storage);
		return img;
	}
	
	IplImage* detectAllCars(IplImage* img){
		
		/* points to represent car' location */
		CvPoint point1, point2;
		
		/* detect objects */
		CvMemStorage* storage = cvCreateMemStorage(0);		
		CvSeq* cars = cvHaarDetectObjects(img, cascade, storage, HAAR_SCALE_FACTOR, HAAR_MIN_NEIGHBORS,
										  CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40));
		
		/* iterate the number of cars found */
		for(int i = 0; i < (cars ? cars->total : 0); i++){
			
			/* allocate a rectangle for drawing a car */
			CvRect* rectangle = (CvRect*)cvGetSeqElem(cars, i);
			
			/* find dimensions of the car (scale if necessary) */
			point1.x = rectangle->x*HAAR_SCALE;
			point2.x = (rectangle->x+rectangle->width)*HAAR_SCALE;
			point1.y = rectangle->y*HAAR_SCALE;
			point2.y = (rectangle->y+rectangle->height)*HAAR_SCALE;
			
			/* draw the rectangle on the image */
			cvRectangle(img, point1, point2, CV_RGB(255,0,0), RECTANGLE_THICKNESS, RECTANGLE_LINETYPE, RECTANGLE_SHIFT);							
		}
		
		cvReleaseMemStorage(&storage);
		return img;
	}
	
	bool isBehindOfCar(CvPoint p1, CvPoint p2, IplImage* img){
		
		CvPoint leftP1, leftP2;
		CvPoint rightP1, rightP2;
		
		leftP1.x = p1.x;
		leftP1.y = p1.y + (3 * (p2.y - p1.y)) / 4;		
		leftP2.x = p1.x + (p2.x - p1.x) / 2;
		leftP2.y = p2.y;
		
		rightP1.x = leftP2.x;
		rightP1.y = leftP1.y;
		rightP2.x = p2.x;
		rightP2.y = p2.y;
		
		/* detect shadow below the car */
		bool shadow = false;
		for (int i = leftP1.x; i < rightP2.x; i++) {
			for (int j = leftP1.y; j < rightP2.y; j++) {
				
				CvScalar s = cvGet2D(img, j, i); 
				if (s.val[0] < SHADOW_THRESH  && s.val[1] < SHADOW_THRESH && s.val[2] < SHADOW_THRESH){
					shadow = true;
					break;
				}				
			}
		}
		
		if(shadow)
			return true;
		else
			return false;	
	}
	
public:
	
	HaarImproved(CvHaarClassifierCascade* cascade){this->cascade = cascade;}
	
	void processFromImages(string imagesDirPath){
		
		/* create a posix directory structure */
		DIR* dir = opendir(imagesDirPath.c_str());
		if(dir != NULL){
			
			struct dirent* dirEntry;
			
			cvNamedWindow(WINDOW_TITLE.c_str(), CV_WINDOW_AUTOSIZE);
			cvNamedWindow(ANOTHER_WINDOW_TITLE.c_str(), CV_WINDOW_AUTOSIZE);
			
			IplImage* image = NULL;
			IplImage* cloneImage = NULL;
			
			/* iterate the directory listing */
			while((dirEntry = readdir(dir)) != NULL){
				
				string filename = dirEntry->d_name;				
				if(filename != "." && filename != ".."){
					
					/* add a slash at the end of directory if not supplied */
					if (*(imagesDirPath.rbegin()) != '/')			
						imagesDirPath.append("/");	
					
					string path = imagesDirPath + filename;					
					
					/* load the image */					
					if((image = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_UNCHANGED)) !=NULL){
						
						image = detectOnlyCars(image);
						
						cloneImage = cvCloneImage(image);
						cloneImage = detectAllCars(cloneImage);
						
						cvShowImage(ANOTHER_WINDOW_TITLE.c_str(), cloneImage);
						cvShowImage(WINDOW_TITLE.c_str(), image);
						cvWaitKey(0);
					}					
				}
			}
			
			/* release resources */
			cvReleaseImage(&image);
			cvReleaseImage(&cloneImage);
			cvDestroyWindow(WINDOW_TITLE.c_str());
			cvDestroyWindow(ANOTHER_WINDOW_TITLE.c_str());
		}
	}
	
	void processFromVideo(string videoPath){
		
		CvCapture* capture = cvCaptureFromAVI(videoPath.c_str());
		cvNamedWindow(WINDOW_TITLE.c_str(), CV_WINDOW_AUTOSIZE);
		
		if(capture){
			
			IplImage* image = NULL;			
			while((image = cvQueryFrame(capture)) != NULL){								
				
				image = detectOnlyCars(image);				
				
				cvShowImage(WINDOW_TITLE.c_str(), image);				
				if(cvWaitKey(10) >= 0)
					break;				
			}
			
			/* release resources */			
			cvReleaseImage(&image);
			cvReleaseCapture(&capture);
			cvDestroyWindow(WINDOW_TITLE.c_str());
		}
	}
};

int main(int argc, char* argv[]){
	if (argc == 4) {
		
		string cascadePath = argv[1];
		
		/* load haar classfier cascade */
		CvHaarClassifierCascade* cascade = 0;
		cascade = (CvHaarClassifierCascade*)cvLoad(cascadePath.c_str(), 0, 0, 0);
		
		if(cascade){
			
			HaarImproved *haar = new HaarImproved(cascade);			
			if (!strcmp(argv[2],"-i")) {
				string imagesDirPath = argv[3];
				haar->processFromImages(imagesDirPath);
			}			
			if (!strcmp(argv[2],"-v")) {
				string videoPath = argv[3];
				haar->processFromVideo(videoPath);
			}			
			delete haar;					
		}else{
			
			cout << "could not load the cascade classfier" << endl;
			cout << cascadePath;
		}		
	}else{
		cout << "usage: haar <cascade xml file> <flag> <source>" << endl;
		cout << "<flag>: -v then <source> is video filename" << endl;
		cout << "<flag>: -i then <source> is images directory" << endl;
	}
}

const string HaarImproved::WINDOW_TITLE = "Improved Haar";
const string HaarImproved::ANOTHER_WINDOW_TITLE = "Haar";
