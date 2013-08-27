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
#include <stdlib.h>

using namespace Robot;

//-----cvimage
Mat VisionStatus::VideoFrame;

//cvSetCaptureProperty(VideoFrame, CV_CAP_PROP_FRAME_WIDTH, 320);
//cvSetCaptureProperty(VideoFrame, CV_CAP_PROP_FRAME_HEIGHT, 240);
//-----ObjCut
unsigned char VisionStatus::Blue_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Green_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Red_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
int VisionStatus::Xmax(0),VisionStatus::Xmin(0),VisionStatus::Ymax(0),VisionStatus::Ymin(0);
int VisionStatus::PointCnt(0);
//-----door number status
bool VisionStatus::doordet_enable = false;
bool VisionStatus::sendimg_enable = false;
bool VisionStatus::Flagdoor_red = false;
bool VisionStatus::Flagdoor_green = false;
bool VisionStatus::Flagdoor_blue = false;

//-----ball posistion
int VisionStatus::Red_X(0);
int VisionStatus::Red_Y(0);
int VisionStatus::Green_X(0);
int VisionStatus::Green_Y(0);
int VisionStatus::Blue_X(0);
int VisionStatus::Blue_Y(0);

VisionStatus::ColorRange VisionStatus::hsvBlueRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvRedRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvGreenRange = {0.51, 0.15, 1.0, 0.0, 1.0, 0.0};

//------ FaceData ----------------------------------------------------------
//------ FaceDetection ----------------------------------------------------------


//------ ImgProcess -----------------------------------------------------------
Mat VisionStatus::Favg_Face_20cm;
float VisionStatus::FeatureFaceW_20cm[40*40];
Mat VisionStatus::Favg_Face_50cm;
float VisionStatus::FeatureFaceW_50cm[40*40];
float VisionStatus::cvGet2D_1(0);
float VisionStatus::cvGet2D_2(0);
float VisionStatus::cvGet2D_3(0);
float VisionStatus::cvGet2D_4(0);
float VisionStatus::cvGet2D_5(0);
float VisionStatus::cvGet2D_6(0);
float VisionStatus::cvGet2D_7(0);
float VisionStatus::cvGet2D_8(0);
float VisionStatus::cvGet2D_9(0);
float VisionStatus::cvGet2D_10(0);
float VisionStatus::cvGet2D_11(0);
float VisionStatus::cvGet2D_12(0);

//------ ImgProcess ----------------------------------------------------------
int VisionStatus::Grandfa_counter(0);
int VisionStatus::Grandma_counter(0);
int VisionStatus::Mother_counter(0);
int VisionStatus::Father_counter(0);
int VisionStatus::Girl_counter(0);
int VisionStatus::Boy_counter(0);
int VisionStatus::Thief_counter(0);


