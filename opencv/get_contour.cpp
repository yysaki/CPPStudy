#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <cv.h>
#include <highgui.h>

#include "get_contour.h"

int main(int argc, char **argv){
	double area;
	CvRect rect;
	int c;

	CvCapture* capture = 0;

	IplImage* frame = 0; // キャプチャ画像
	IplImage* mask = 0; // 指定値によるmask
	IplImage* contour_img = 0; // GetLargestContour()結果

	// 赤系のHSV色
	uchar minH = 110, maxH = 130;
	uchar minS = 120, maxS = 255;
	uchar minV = 30,  maxV = 255;


	if(argc == 1 || (argc == 2 && strlen (argv[1]) == 1 && isdigit(argv[1][0])))
		capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);

	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, 320); // デフォルト320
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, 240); // デフォルト240

	frame = cvQueryFrame(capture);
	mask = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	contour_img = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3 );

	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "contour", CV_WINDOW_AUTOSIZE );

	while(1) {
		frame = cvQueryFrame(capture);
		cvShowImage("src", frame);

		GetMaskHSV(frame, mask, minH, maxH, minS, maxS, minV, maxV);

		GetLargestContour(mask,contour_img, &area, &rect);

		if(area>0){
			printf("area:%f\n", area);
			printf("rect x:%d, y:%d, width:%d, height:%d \n", rect.x, rect.y, rect.width, rect.height);
			printf("rect center: (%d, %d)\n", rect.x+(rect.width/2), rect.y+(rect.height/2));
		}

		cvShowImage( "contour", contour_img );

		c = cvWaitKey(10);
		if(c > 0){
			break;
		}
	}
	cvDestroyWindow("src");
	cvDestroyWindow("contour");

	cvReleaseImage(&frame);
	cvReleaseImage(&mask);
	cvReleaseImage(&contour_img);

	cvReleaseCapture(&capture);

	return 0;
}
