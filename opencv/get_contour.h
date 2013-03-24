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
#define THRESHOLD 10      //  2�l���̍ۂ�臒l
#define THRESHOLD_MAX_VALUE 255 //  2�l���̍ۂɎg�p����ő�l
#define CONTOUR_MAX_LEVEL 1 //  �`�悳���֊s�̍ő僌�x��
#define LINE_THICKNESS  5   //  ���̑���
#define LINE_TYPE 8     //  ���̎��

/* for interpolate */
#define ITERATIONS  4 //  �c���A���k�̉�

/*
 * HSV�ɂ���l���摜�̎擾
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

	//HSV�ɕϊ�
	cvCvtColor(src, tmp, CV_RGB2HSV);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
			tmp->widthStep,cvGetSize(tmp), x, y, tmp->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
			mask->widthStep, cvGetSize(mask), x, y, mask->origin);

	for(y = 0; y < tmp->height; y++) {
		for(x = 0; x < tmp->width; x++) {
			p_src = CV_MOVE_TO(pos_src, x, y, 3);
			p_dst = CV_MOVE_TO(pos_dst, x, y, 3);

			H = p_src[0];//0����180
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
	//�c����ITERATIONS��s��
	cvDilate( skinImage, temp, NULL, ITERATIONS );

	//���k��ITERATIONS��s��
	cvErode( temp, skinImage, NULL, ITERATIONS );

	cvReleaseImage(&temp);
}

/*
 * �ő�֊s�A�O�ڋ�`�̕`��
 * ���S���W��Ԃ�
 * ���݂��Ȃ��ꍇ *retArea��0��Ԃ�
 */
void GetLargestContour(IplImage *src, IplImage *ret, double *retArea, CvRect *retRect){
	IplImage *src_cpy = 0;
	IplImage *gray = 0;
	IplImage *binary = 0;

	src_cpy = cvCloneImage(src);
	gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );   //  �O���[�X�P�[���摜�pIplImage
	binary = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 ); //  2�l�摜�pIplImage

	CvMemStorage* storage = cvCreateMemStorage( 0 );  //  ���o���ꂽ�֊s��ۑ�����̈�
	CvSeq* contours = NULL;   //  �֊s�ւ̃|�C���^
	CvSeq *it, *ptr;
	CvPoint ans; // �O�ڋ�`�̒��S���W

	interpolate(src_cpy); // �c���Ǝ��k

	//�֊s���o����
	cvCvtColor( src_cpy, gray, CV_BGR2GRAY );
	cvThreshold( gray, binary, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

	cvCopy(src, ret, NULL);

	int contour_num = cvFindContours(binary, storage, &contours,
			sizeof( CvContour ), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint( 0, 0 ));

//	printf("contour_num: %d\n", contour_num); // �擾�����֊s�̑���

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

//		printf("maxArea: %f\n",  maxArea); // �֊s�O�ڋ�`�̒��S���W
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

	if(!(contour_num && maxArea)){ // �֊s�̑��݂��Ȃ��ꍇ 0 ��Ԃ�
		*retArea = 0;
	}
}

#endif /* GET_CONTOUR_H_ */
