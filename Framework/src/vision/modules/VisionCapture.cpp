/*
 *   VisionCapture.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include "Status.h"
#include "VisionCapture.h"
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "ReadVision.h"

using namespace Robot;
using namespace std;
using namespace cv;

VisionCapture* VisionCapture::m_UniqueInstance = new VisionCapture();


VisionCapture::VisionCapture()
{
	
}

VisionCapture::~VisionCapture()
{
}

void VisionCapture::Initialize()
{
	Status::capture = cvCaptureFromCAM( -1 );
}

void VisionCapture::LoadINISettings(minIni* ini)
{
    LoadINISettings(ini, TASK_SECTION);
}

void VisionCapture::LoadINISettings(minIni* ini, const std::string &section)
{
    double value = INVALID_VALUE;

    //if((value = ini->getd(section, "pan_p_gain", INVALID_VALUE)) != INVALID_VALUE)  m_Pan_p_gain = value;
    //if((value = ini->getd(section, "pan_d_gain", INVALID_VALUE)) != INVALID_VALUE)  m_Pan_d_gain = value;
    //if((value = ini->getd(section, "tilt_p_gain", INVALID_VALUE)) != INVALID_VALUE) m_Tilt_p_gain = value;
    //if((value = ini->getd(section, "tilt_d_gain", INVALID_VALUE)) != INVALID_VALUE) m_Tilt_d_gain = value;
    //if((value = ini->getd(section, "left_limit", INVALID_VALUE)) != INVALID_VALUE)  m_LeftLimit = value;
    //if((value = ini->getd(section, "right_limit", INVALID_VALUE)) != INVALID_VALUE) m_RightLimit = value;
    //if((value = ini->getd(section, "top_limit", INVALID_VALUE)) != INVALID_VALUE)   m_TopLimit = value;
    //if((value = ini->getd(section, "bottom_limit", INVALID_VALUE)) != INVALID_VALUE)m_BottomLimit = value;
    //if((value = ini->getd(section, "pan_home", INVALID_VALUE)) != INVALID_VALUE)    m_Pan_Home = value;
    //if((value = ini->getd(section, "tilt_home", INVALID_VALUE)) != INVALID_VALUE)   m_Tilt_Home = value;
}

void VisionCapture::SaveINISettings(minIni* ini)
{
    SaveINISettings(ini, TASK_SECTION);
}

void VisionCapture::SaveINISettings(minIni* ini, const std::string &section)
{
    //ini->put(section,   "pan_p_gain",   m_Pan_p_gain);
    //ini->put(section,   "pan_d_gain",   m_Pan_d_gain);
    //ini->put(section,   "tilt_p_gain",  m_Tilt_p_gain);
    //ini->put(section,   "tilt_d_gain",  m_Tilt_d_gain);
    //ini->put(section,   "left_limit",   m_LeftLimit);
    //ini->put(section,   "right_limit",  m_RightLimit);
    //ini->put(section,   "top_limit",    m_TopLimit);
    //ini->put(section,   "bottom_limit", m_BottomLimit);
    //ini->put(section,   "pan_home",     m_Pan_Home);
    //ini->put(section,   "tilt_home",    m_Tilt_Home);
}

void VisionCapture::Process()
{
	Mat frame;
	frame = cvQueryFrame( Status::capture);
	IplImage img1 = IplImage (frame);
	Status::a = img1.imageData[0];

	//printf("%d",ReadVision::hello);
}
