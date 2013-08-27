/*
 *   FaceDetection_Stage2.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "FaceDetection_Stage2.h"
#include "VisionStatus.h"
#include "StrategyStatus.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.1415926
#define GET_FACE_CNT 2

using namespace Robot;
using namespace std;
using namespace cv;

FaceDetection_Stage2* FaceDetection_Stage2::m_UniqueInstance = new FaceDetection_Stage2();

FaceDetection_Stage2::FaceDetection_Stage2()
{
}

FaceDetection_Stage2::~FaceDetection_Stage2()
{
}

void FaceDetection_Stage2::Initialize()
{

}

void FaceDetection_Stage2::Process()
{
    string face_cascade_name = "../../../Data/jpg/haarcascade_frontalface_alt.xml";
    CascadeClassifier face_cascade;
    face_cascade.load( face_cascade_name );
    std::vector<Rect> faces;
    Mat Video = VisionStatus::VideoFrame;
    Mat Video_gray;
    cvtColor( Video, Video_gray, CV_BGR2GRAY );
    //equalizeHist( Video_gray, Video_gray );
    face_cascade.detectMultiScale(Video_gray, faces,1.1,2, 0|CV_HAAR_SCALE_IMAGE, Size(40,40));
	if( faces.size() >= 1 )
	{
		cv::imwrite("Thief.jpg",VisionStatus::VideoFrame);
	}
}
