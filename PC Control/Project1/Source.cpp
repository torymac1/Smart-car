#pragma once
#include <process.h>
#include "csocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include "opencv2/core/core.hpp"  
#include "opencv2/features2d/features2d.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/calib3d/calib3d.hpp" 
#include <fstream>
//using namespace cv;
using namespace std;


#define CMD_Stop	"FF000000FF"
#define CMD_Forward "FF000100FF"
#define CMD_Back	"FF000200FF"
#define CMD_Left	"FF000300FF"
#define CMD_Right	"FF000400FF"
#define CMD_FLeft	"FF000500FF"
#define CMD_FRight  "FF000700FF"
#define CMD_BLeft	"FF000600FF"
#define CMD_BRight  "FF000800FF"
#define CMD_Lspee_10  "FF020102FF"
#define CMD_Lspee_01  "FF020101FF"
#define CMD_Rspee_10  "FF020202FF"
#define CMD_Rspee_03  "FF020200FF"
#define CMD_Line  "FF130200FF"
#define CMD_Left_Speed_8  "FF020105FF"
#define CMD_Right_Speed_8  "FF020205FF"
#define CMD_Barrier  "FF130400FF"
class RobotCar
{
private:
	CSocket s;

public:

	void turnRight()
	{
		s.SendByte(CMD_Right);
		Sleep(440);
		s.SendByte(CMD_Stop);
		Sleep(100);
	}

	
	
	void shutdown() {
		Sleep(100);
	}

	void line()
	{
		s.SendByte(CMD_Line);
	}

	void barrier()
	{
		s.SendByte(CMD_Barrier);
	}

	void turnRight(int ms){
		s.SendByte(CMD_Right);
		Sleep(ms);
		s.SendByte(CMD_Stop);
		Sleep(100);
		cout << "turn right " << ms << endl;
	}
	void Go(int ms)
	{
		s.SendByte(CMD_Forward);
		Sleep(ms);
		s.SendByte(CMD_Stop);
		Sleep(200);
	}
	void turnLeft()
	{
		s.SendByte(CMD_Left);
		Sleep(440);
		s.SendByte(CMD_Stop);
		Sleep(100);
	}
	void turnLeft(int ms)
	{
		s.SendByte(CMD_Left);
		Sleep(ms);
		s.SendByte(CMD_Stop);
		Sleep(100);
	}
	void Go()
	{
		s.SendByte(CMD_Forward);
		Sleep(440);
		s.SendByte(CMD_Stop);
		Sleep(100);
	}

	void carsleep(int ms) {
		Sleep(ms);
	}

	void Go_speed_08()
	{
		s.SendByte(CMD_Left_Speed_8);
		s.SendByte(CMD_Right_Speed_8);
	}

	
};





int *red_1(const char *filename)
{
	IplImage * src = cvLoadImage(filename, 1);
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
			if (temp.val[2] - temp.val[1]>70 && temp.val[2] - temp.val[0]>70) {
				red.val[0] = 0;
				red.val[1] = 0;
				red.val[2] = 0;
				m += 1;
				x1 = x1 + j;
				y1 = y1 + i;
			}
			else {
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
	if (m != 0){
		x = x1 / m;
		y = y1 / m;
	}
	else {
		int *zz = new int[2];
		zz[0] = 0;
		zz[1] = 0;
		return zz;
	}
	src = NULL;
	dst = NULL;
	int *zz = new int[2];
	zz[0] = x;
	zz[1] = y;
	cout << zz[0] << endl;
	cout << zz[1] << endl;
	return zz;
}


int main() {
	RobotCar car;
	//
	
	
	
	car.Go_speed_08();
	car.barrier();
	//car.line();
	
	/*
	const char *spic1 = "pi1.jpg";
	const char *spic2 = "pi2.jpg";
	mypicture pic;
	
	pic.initial();
	while(!pic.receiveImageOneTime(spic1));
	*/

}

