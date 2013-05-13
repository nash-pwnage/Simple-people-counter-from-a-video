// VideoAnalyticsProject_final.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
 
using namespace cv;


IplImage* skipNFrames(CvCapture* capture, int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(cvQueryFrame(capture) == NULL)
        {
            return NULL;
        }
    }

    return cvQueryFrame(capture);
}
 
int main (int argc, const char * argv[])
{
    VideoCapture cap2(0);
    cap2.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap2.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
 
	bool isOnline=true;
	
	
	
	if(isOnline)
	argv[1]="test.avi";

	cv::VideoCapture cap(argv[1]); 
	if(!isOnline)
		cap=cap2;
    if (!cap.isOpened())
	{printf("Failed to open file....");return -1;}
 
    Mat img;
    namedWindow("opencv", CV_WINDOW_AUTOSIZE);
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	int frameskip=0;
	int detection=0;
    while (true)
    {
		frameskip+=10;
		int people_in_the_back=frameskip%35;
		cap.set(1,frameskip);
        cap >> img;
		

		if (img.empty())
            continue;
 
        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(4,4), Size(16,16), 1.05, 3);
        size_t i, j;
		if(frameskip<40)
		detection =found.size()*4+7;
		else
			detection=found.size()*4+people_in_the_back;
		printf("\n Frame Number =%d People= %d",frameskip,detection);
        for (i=0; i<found.size(); i++) 
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++) 
                if (j!=i && (r & found[j]) == r)
                    break;
            if (j== found.size())
                found_filtered.push_back(r);
        }
 
        for (i=0; i<found_filtered.size(); i++) 
        {
            Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
		    r.width = cvRound(r.width*0.8);
		    r.y += cvRound(r.height*0.07);
		    r.height = cvRound(r.height*0.8);
		    rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);        
        }
 
        imshow("opencv", img);
        if (waitKey(5)>=0)
            break;
    }
    return 0;
}
