#ifndef GET_CONTOUR_H_
#define GET_CONTOUR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <cv.h>
#include <highgui.h>

void GetMaskHSV(IplImage* src, IplImage* mask, uchar minH, uchar maxH, uchar minS, uchar maxS, uchar minV, uchar maxV);
void interpolate( IplImage *skinImage);
void GetLargestContour(IplImage *src, IplImage *ret, double *retArea, CvRect *retRect);


/* for cvFindContours */
#define THRESHOLD 10      //  2値化の際の閾値
#define THRESHOLD_MAX_VALUE 255 //  2値化の際に使用する最大値
#define CONTOUR_MAX_LEVEL 1 //  描画される輪郭の最大レベル
#define LINE_THICKNESS  5   //  線の太さ
#define LINE_TYPE 8     //  線の種類

/* for interpolate */
#define ITERATIONS  4 //  膨張、収縮の回数

/*
 * HSVによる二値化画像の取得
 */
void GetMaskHSV(IplImage* src, IplImage* mask, uchar minH, uchar maxH, uchar minS, uchar maxS, uchar minV, uchar maxV)
{
	int erosions = 1;
	int dilations = 1;
	int x = 0, y = 0;
	uchar H, S, V;

	CvPixelPosition8u pos_src, pos_dst;
	uchar* p_src;
	uchar* p_dst;
	IplImage* tmp;
	//counter
	int count = 0;
	double ratio;
	tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);

	//HSVに変換
	cvCvtColor(src, tmp, CV_RGB2HSV);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
			tmp->widthStep,cvGetSize(tmp), x, y, tmp->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
			mask->widthStep, cvGetSize(mask), x, y, mask->origin);

	for(y = 0; y < tmp->height; y++) {
		for(x = 0; x < tmp->width; x++) {
			p_src = CV_MOVE_TO(pos_src, x, y, 3);
			p_dst = CV_MOVE_TO(pos_dst, x, y, 3);

			H = p_src[0];//0から180
			S = p_src[1];
			V = p_src[2];

			if( minH <= H && H <= maxH &&
					minS <= S && S <= maxS &&
					minV <= V && V <= maxV
			) {
				count++; //count
				p_dst[0] = 255;
				p_dst[1] = 255;
				p_dst[2] = 255;
			} else {
				p_dst[0] = 0;
				p_dst[1] = 0;
				p_dst[2] = 0;
			}
		}
	}

	if(erosions > 0)  cvErode(mask, mask, 0, erosions);
	if(dilations > 0) cvDilate(mask, mask, 0, dilations);

	//   std::cout << "width:" << tmp->width << ",height:" << tmp->height << ",all:" << tmp->width*tmp->height << ",count:" << count <<std::endl;

	cvReleaseImage(&tmp);
}

void interpolate( IplImage *skinImage) {
	IplImage *temp = 0;
	temp = cvCreateImage( cvGetSize( skinImage ),IPL_DEPTH_8U,3);
	//膨張をITERATIONS回行う
	cvDilate( skinImage, temp, NULL, ITERATIONS );

	//収縮をITERATIONS回行う
	cvErode( temp, skinImage, NULL, ITERATIONS );

	cvReleaseImage(&temp);
}

/*
 * 最大輪郭、外接矩形の描画
 * 中心座標を返す
 * 存在しない場合 *retAreaが0を返す
 */
void GetLargestContour(IplImage *src, IplImage *ret, double *retArea, CvRect *retRect){
	IplImage *src_cpy = 0;
	IplImage *gray = 0;
	IplImage *binary = 0;

	src_cpy = cvCloneImage(src);
	gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );   //  グレースケール画像用IplImage
	binary = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 ); //  2値画像用IplImage

	CvMemStorage* storage = cvCreateMemStorage( 0 );  //  抽出された輪郭を保存する領域
	CvSeq* contours = NULL;   //  輪郭へのポインタ
	CvSeq *it, *ptr;
	CvPoint ans; // 外接矩形の中心座標

	interpolate(src_cpy); // 膨張と収縮

	//輪郭抽出操作
	cvCvtColor( src_cpy, gray, CV_BGR2GRAY );
	cvThreshold( gray, binary, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

	cvCopy(src, ret, NULL);

	int contour_num = cvFindContours(binary, storage, &contours,
			sizeof( CvContour ), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint( 0, 0 ));

//	printf("contour_num: %d\n", contour_num); // 取得した輪郭の総数

	double area, maxArea;
	maxArea = 0;
	if(contour_num){
		it = contours;
		while(it != NULL){
			area = cvContourArea(it, CV_WHOLE_SEQ, 0);
			if(maxArea<area){
				maxArea = area;
				ptr = it;
			}
			it = it->h_next;
		}

//		printf("maxArea: %f\n",  maxArea); // 輪郭外接矩形の中心座標
		if (maxArea) {
			*retArea = maxArea;
			*retRect = cvBoundingRect (ptr, 0);

			cvDrawContours(ret, ptr, CV_RGB (0, 0, 255),
					CV_RGB (0, 0, 255), 0, LINE_THICKNESS, LINE_TYPE, cvPoint( 0, 0 ));
			cvRectangle (ret, cvPoint (retRect->x, retRect->y),
					cvPoint (retRect->x + retRect->width, retRect->y + retRect->height), CV_RGB (0, 255, 0), 2, 8, 0);
		}
	}

	cvReleaseImage( &gray );
	cvReleaseImage( &binary );
	cvReleaseImage( &src_cpy);
	cvReleaseMemStorage(&storage);

	if(!(contour_num && maxArea)){ // 輪郭の存在しない場合 0 を返す
		*retArea = 0;
	}
}

#endif /* GET_CONTOUR_H_ */
