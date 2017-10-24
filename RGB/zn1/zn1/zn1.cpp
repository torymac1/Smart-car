#pragma once
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include "opencv2/core/core.hpp"  
#include "opencv2/features2d/features2d.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/calib3d/calib3d.hpp" 
#include <fstream>
using namespace cv;
using namespace std;


/*int red(const char *s, const char *d) {
	
	IplImage * src = cvLoadImage(s, 1);
	IplImage * dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	float m = 0;
	float h = 0;
	int w = 0;
	CvScalar temp; 
	CvScalar red;
	for (int i = 0; i < 240; ++i) {
		for (int j = 0; j < 320; ++j) {
			temp = cvGet2D(src, i, j);
			if (temp.val[2] - temp.val[1]>40 && temp.val[2] - temp.val[0]>40) {
				w = w + j;
				h = h + i;
				m = m + 1;
				red.val[0] = 0; 
				red.val[1] = 0;
				red.val[2] = 0;
			}
			else{
				red.val[0] = 255;
				red.val[1] = 255;
				red.val[2] = 255;
			}
			cvSet2D(dst, i, j, red);
		}
	}
	cvSaveImage(d, dst);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	src = NULL;
	dst = NULL;
	return 0;
}*/

int *red_1()
{
	IplImage * src = cvLoadImage("pic.jpg", 1);
	IplImage * dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	CvScalar temp; 
	CvScalar red;
	int m = 0;
	int x1 = 0;
	int y1 = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 240; ++i) {
		for (int j = 0; j < 320; ++j) {
			temp = cvGet2D(src, i, j);
			if (temp.val[2] - temp.val[1]>40 && temp.val[2] - temp.val[0]>40) {
				red.val[0] = 0; 
				red.val[1] = 0;
				red.val[2] = 0;
				m += 1;
				x1 = x1+j;
				y1 = y1+i;
			}
			else{
				red.val[0] = 255;
				red.val[1] = 255;
				red.val[2] = 255;
			}
			cvSet2D(dst, i, j, red);
			//			
		}
	}
	cvSaveImage("test.jpg", dst);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	x=x1/m;
	y=y1/m;
	src = NULL;
	dst = NULL;
	int *zz=new int[2];
	zz[0]=x;
	zz[1]=y;
	cout<<zz[0]<<endl;
	cout<<zz[1]<<endl;
	return zz;
}

int main()
{
	/*
	IplImage * src = cvLoadImage("pic.jpg", 1);
	IplImage * dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	CvScalar temp; 
	CvScalar red;
	int m = 0;
	int x1 = 0;
	int y1 = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 240; ++i) {
		for (int j = 0; j < 320; ++j) {
			temp = cvGet2D(src, i, j);
			if (temp.val[2] - temp.val[1]>40 && temp.val[2] - temp.val[0]>40) {
				red.val[0] = 0; 
				red.val[1] = 0;
				red.val[2] = 0;
				m += 1;
				x1 = x1+j;
				y1 = y1+i;
			}
			else{
				red.val[0] = 255;
				red.val[1] = 255;
				red.val[2] = 255;
			}
			cvSet2D(dst, i, j, red);
			//			
		}
	}
	cvSaveImage("test.jpg", dst);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	x=x1/m;
	y=y1/m;
	src = NULL;
	dst = NULL;
	return m;
	*/
	int *b;
	b=red_1();
	cout<<b[0]<<endl<<b[1];
}

