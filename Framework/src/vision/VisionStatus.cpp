/*
 *   VisionStatus.cpp
 *
 *   Author: Wu Chih-En
 *
 */
#define _USE_MATH_DEFINES


#include <math.h>
#include <stdio.h>
#include "VisionStatus.h"



using namespace Robot;

//unsigned char *VisionStatus::pImage = new unsigned char[3 * VisionStatus::ImageWidth * VisionStatus::ImageHeight];
//-----cvimage
cv::Mat VisionStatus::frame;
cv::Mat VisionStatus::send_frame;

//-----ObjCut
unsigned char VisionStatus::Blue_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Green_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Red_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];

//-----ball posistion
int VisionStatus::Red_X(0);
int VisionStatus::Red_Y(0);
int VisionStatus::Green_X(0);
int VisionStatus::Green_Y(0);
int VisionStatus::Blue_X(0); 
int VisionStatus::Blue_Y(0); 

VisionStatus::ColorRange VisionStatus::hsvBlueRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvRedRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvGreenRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};


int VisionStatus::ColorDeep(256);
int VisionStatus::xImageCenter(320);
int VisionStatus::yImageCenter(240);
int VisionStatus::GlobalScanMagnEnd(237);

float VisionStatus::AngularToRadian(M_PI / 180.0);
float VisionStatus::RadianToAngular(180.0 / M_PI);
float VisionStatus::ComputeCenterAngle(float AngleStartPoint, float AngleEndPoint)
{
	float tmpAngle;

	if(AngleEndPoint < AngleStartPoint)
	{
		if(AngleStartPoint < 0.0)
			AngleStartPoint += (2.0 * M_PI);
		if(AngleEndPoint < 0.0)
			AngleEndPoint += (2.0 * M_PI);
	}
	tmpAngle = (AngleEndPoint - AngleStartPoint) / 2.0 + AngleStartPoint;

	//else
	//{
	//        tmpAngle = (AngleEndPoint + AngleStartPoint) / 2.0;
	//}

	if(tmpAngle > M_PI)
		tmpAngle -= (2.0 * M_PI);

	return tmpAngle;
}




