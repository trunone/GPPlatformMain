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
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n");};
	CvMat *FFW;
	CvMat *PW;
	CvMat *SumWP;
	unsigned char Buffer1[800*600*3];
	unsigned char Buffer2[40*40*3];
	unsigned char People[40*40*3];
	unsigned char PeopleD[40*40*3];
	float PeopleDW[40*40*3];
	int b=0,w=0,h=0;
	float Iall=0,Igray,Iavg,favg,fd;
	float I=0,Ih=0;
   	std::vector<Rect> faces;
	IplImage img1 = IplImage (VisionStatus::VideoFrame);
   	Mat frame_gray;
   	cvtColor( VisionStatus::VideoFrame, frame_gray, CV_BGR2GRAY );
   	equalizeHist( frame_gray, frame_gray );
	//imwrite("a.jpg", frame_gray);
   	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(0,0));
   	for( size_t i = 0; i < faces.size(); i++ )
    {
        b=0, Iall=0;
		w=faces[i].width;
		h=faces[i].height;
		for(int a=faces[i].y;a<faces[i].y + faces[i].height;a++){
			for(int j=faces[i].x;j<faces[i].x + faces[i].width;j++){
				Buffer1[b] = img1.imageData[a*img1.widthStep+j*3];
				Buffer1[b+1] = img1.imageData[a*img1.widthStep+j*3+1];
				Buffer1[b+2] = img1.imageData[a*img1.widthStep+j*3+2];
				b=b+3;
		    }
		}
		for(int a=0;a<40;a++){
		    for(int j=0;j<40;j++){
		        Buffer2[a*40*3+j*3]=Buffer1[(a*h/40)*w*3+(j*w/40)*3];
		        Buffer2[a*40*3+j*3+1]=Buffer1[(a*h/40)*w*3+(j*w/40)*3+1];
		        Buffer2[a*40*3+j*3+2]=Buffer1[(a*h/40)*w*3+(j*w/40)*3+2];
		    }
		}
		for(int a=0;a<40;a++){
		    for(int j=0;j<40;j++){
		        Igray =  0.114*Buffer2[a*40*3+j*3]
						   + 0.587*Buffer2[a*40*3+j*3+1]
						   + 0.299*Buffer2[a*40*3+j*3+2];
		        Iall = Iall+Igray;
		    }
		}
		Iavg = Iall/(40*40);
		I=128-Iavg;
		for(int a=0;a<40;a++){
		    for(int j=0;j<40;j++){
		        Igray =  0.114*Buffer2[a*40*3+j*3]
						   + 0.587*Buffer2[a*40*3+j*3+1]
						   + 0.299*Buffer2[a*40*3+j*3+2];
		        Ih=I+Igray;
		        if(Ih>255)Ih=255;if(Ih<0)Ih=0;
		        Buffer2[a*40*3+j*3]=Ih;
		        Buffer2[a*40*3+j*3+1]=Ih;
		        Buffer2[a*40*3+j*3+2]=Ih;
		    }
		}
		for(int a=0;a<40*40*3;a++)People[a] = Buffer2[a];
		for(int a=0;a<40;a++){
			for(int j=0;j<40;j++){
				fd=People[a*40*3+j*3]-VisionStatus::Favg[a*40*3+j*3];
				if(fd<0)fd=0;
				PeopleD[a*40*3+j*3]=fd;
				PeopleD[a*40*3+j*3+1]=fd;
				PeopleD[a*40*3+j*3+2]=fd;
			}
		}
		for(int a=0;a<40*40;a++)PeopleDW[a]=PeopleD[a*3];
		FFW=cvCreateMat(1,40*40,CV_32FC1);
		cvSetData(FFW,VisionStatus::FeatureFaceW,FFW->step);
		PW=cvCreateMat(40*40,1,CV_32FC1);
		cvSetData(PW,PeopleDW,PW->step);
		SumWP=cvCreateMat(1,1,CV_32FC1);
		cvMatMul(FFW,PW,SumWP);
		//printf("%f\n",cvGet2D(SumWP,0,0).val[0]);
		if((cvGet2D(SumWP,0,0).val[0]<1550000)&&(cvGet2D(SumWP,0,0).val[0]>100000)){
			VisionStatus::counter1++;VisionStatus::counter7=0;VisionStatus::counter3=0;VisionStatus::counter4=0;VisionStatus::counter5=0;VisionStatus::counter6=0;
		}
		else if((cvGet2D(SumWP,0,0).val[0]<28000)&&(cvGet2D(SumWP,0,0).val[0]>19000)){
			VisionStatus::counter1=0;VisionStatus::counter7=0;VisionStatus::counter3++;VisionStatus::counter4=0;VisionStatus::counter5=0;VisionStatus::counter6=0;
		}
		else if((cvGet2D(SumWP,0,0).val[0]<10000)&&(cvGet2D(SumWP,0,0).val[0]>5000)){
			VisionStatus::counter1=0;VisionStatus::counter7=0;VisionStatus::counter3=0;VisionStatus::counter4++;VisionStatus::counter5=0;VisionStatus::counter6=0;
		}
		else if((cvGet2D(SumWP,0,0).val[0]<100000)&&(cvGet2D(SumWP,0,0).val[0]>72000)){
			VisionStatus::counter1=0;VisionStatus::counter7=0;VisionStatus::counter3=0;VisionStatus::counter4=0;VisionStatus::counter5++;VisionStatus::counter6=0;
		}
		else if((cvGet2D(SumWP,0,0).val[0]<72000)&&(cvGet2D(SumWP,0,0).val[0]>65000)){
			VisionStatus::counter1=0;VisionStatus::counter7=0;VisionStatus::counter3=0;VisionStatus::counter4=0;VisionStatus::counter5=0;VisionStatus::counter6++;
		}
		else{
			VisionStatus::counter1=0;VisionStatus::counter7++;VisionStatus::counter3=0;VisionStatus::counter4=0;VisionStatus::counter5=0;VisionStatus::counter6=0;
		}
		if(VisionStatus::counter1==5){
			VisionStatus::counter1=0;
			printf("Grandfa\n"); 
		}
		if(VisionStatus::counter7==5){
			VisionStatus::counter7=0;
			printf("Badman\n");
		}
		if(VisionStatus::counter3==5){
			VisionStatus::counter3=0;
			printf("Mother\n");
		}
		if(VisionStatus::counter4==5){
			VisionStatus::counter4=0;
			printf("Father\n");
		}
		if(VisionStatus::counter5==5){
			VisionStatus::counter5=0;
			printf("Girl\n");
		}
		if(VisionStatus::counter6==5){
			VisionStatus::counter6=0;
			printf("Boy\n");
		}
	}
}
