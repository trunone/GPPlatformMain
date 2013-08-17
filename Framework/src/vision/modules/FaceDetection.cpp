/*
 *   FaceDetection.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "FaceDetection.h"
#include "VisionStatus.h"
#include "StrategyStatus.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.1415926

using namespace Robot;
using namespace std;
using namespace cv;

FaceDetection* FaceDetection::m_UniqueInstance = new FaceDetection();

FaceDetection::FaceDetection()
{
}

FaceDetection::~FaceDetection()
{
}

void FaceDetection::Initialize()
{
	
}

void FaceDetection::Process()
{
	string face_cascade_name = "../../../Data/jpg/haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;
	face_cascade.load( face_cascade_name );
	CvMat *FFW;
	CvMat *PW;
	CvMat *SumWP;
	float PeopleDW[40*40*3];
	int Facedata=0;
	int Iall=0,Iavg=0,I=0;
	std::vector<Rect> faces;
	Mat Video = VisionStatus::VideoFrame;
   	Mat Video_gray;
	Mat Face40size( Size(40,40) , CV_8UC3 );
   	cvtColor( Video, Video_gray, CV_BGR2GRAY );
   	equalizeHist( Video_gray, Video_gray );
	face_cascade.detectMultiScale(Video_gray, faces,1.1,2, 0|CV_HAAR_SCALE_IMAGE, Size(40,40));
   	for( size_t i = 0; i < faces.size(); i++ )
	{
		Facedata=0;
		Mat Face( Size(faces[i].width,faces[i].height) , CV_8UC3 );
		for(int a=faces[i].y;a<faces[i].y + faces[i].height;a++){
			for(int j=faces[i].x;j<faces[i].x+faces[i].width;j++){
				Face.data[Facedata] = Video.data[a*Video.cols*3+(j*3)];
				Face.data[Facedata+1] = Video.data[a*Video.cols*3+(j*3)+1];
				Face.data[Facedata+2] = Video.data[a*Video.cols*3+(j*3)+2];
				Facedata = Facedata+3;
			}	
		}
		Mat Face_Gray;
		cvtColor( Face, Face_Gray, CV_BGR2GRAY );
		for(int a=0;a<40;a++){
			for(int j=0;j<40;j++){
			Face40size.data[a*40*3+j*3] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
			Face40size.data[a*40*3+j*3+1] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
			Face40size.data[a*40*3+j*3+2] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
			}
		}
		Iall=0;
		for(int i=0;i<40*40;i++)Iall = Iall+Face40size.data[i*3];
		Iavg = Iall/(40*40);
		for(int i=0;i<40*40*3;i++){
			I = (128-Iavg)+Face40size.data[i];
			if(I<0)I=0;
			if(I>255)I=255;
			Face40size.data[i] = I;
		}
		int fd = 0;
		for(int i=0;i<40;i++){
			for(int j=0;j<40;j++){
				fd = Face40size.data[i*40*3+j*3] - VisionStatus::Favg_Face.data[i*40*3+j*3];
				if(fd<0)fd=0;
				Face40size.data[i*40*3+j*3]=fd;
				Face40size.data[i*40*3+j*3+1]=fd;
				Face40size.data[i*40*3+j*3+2]=fd;
			}
		}
		for(int a=0;a<40*40;a++)PeopleDW[a]=Face40size.data[a*3];
		FFW=cvCreateMat(1,40*40,CV_32FC1);
		cvSetData(FFW,VisionStatus::FeatureFaceW,FFW->step);
		PW=cvCreateMat(40*40,1,CV_32FC1);
		cvSetData(PW,PeopleDW,PW->step);
		SumWP=cvCreateMat(1,1,CV_32FC1);
		cvMatMul(FFW,PW,SumWP);
		printf("%f\n",cvGet2D(SumWP,0,0).val[0]);
		if(( 10000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<21000 )printf("Grandfa\n");
		else if(( 190000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<280000 )printf("Mother\n");
		else if(( 330000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<445000 )printf("Father\n");
		else if(( 25000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<61000 )printf("Girl\n");
		else if(( 90000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<160000 )printf("Boy\n");
		else printf("Miss\n");	
	
	}
}
