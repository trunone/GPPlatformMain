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
	string face_cascade_name = "../../../jpg/Data/haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;
	CvMat *FFW;
	CvMat *PW;
	CvMat *SumWP;
	unsigned char Buffer1[800*600*3];
	unsigned char Buffer2[40*40*3];
	unsigned char People[40*40*3];
	unsigned char PeopleD[40*40*3];
	float PeopleDW[40*40*3];
	int b=0,w=0,h=0;
	float Iall=0,Igray,Iavg,favg,fd,cp0,cp1,ff,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12;
	float I=0,Ih=0;
   	std::vector<Rect> faces;
	IplImage img1 = IplImage (VisionStatus::VideoFrame);
   	Mat frame_gray;
   	cvtColor( VisionStatus::VideoFrame, frame_gray, CV_BGR2GRAY );
   	equalizeHist( frame_gray, frame_gray );
   	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(40,40));
   	for( size_t i = 0; i < faces.size(); i++ )
    {
        b=0;
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
    Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
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

	d1 = fabs(VisionStatus::cvGet2D_1 - cvGet2D(SumWP,0,0).val[0]);
	d2 = fabs(VisionStatus::cvGet2D_2 - cvGet2D(SumWP,0,0).val[0]);
	d3 = fabs(VisionStatus::cvGet2D_3 - cvGet2D(SumWP,0,0).val[0]);
	d4 = fabs(VisionStatus::cvGet2D_4 - cvGet2D(SumWP,0,0).val[0]);
	d5 = fabs(VisionStatus::cvGet2D_5 - cvGet2D(SumWP,0,0).val[0]);
	d6 = fabs(VisionStatus::cvGet2D_6 - cvGet2D(SumWP,0,0).val[0]);
	d7 = fabs(VisionStatus::cvGet2D_7 - cvGet2D(SumWP,0,0).val[0]);
	d8 = fabs(VisionStatus::cvGet2D_8 - cvGet2D(SumWP,0,0).val[0]);
	d9 = fabs(VisionStatus::cvGet2D_9 - cvGet2D(SumWP,0,0).val[0]);
	d10 = fabs(VisionStatus::cvGet2D_10 - cvGet2D(SumWP,0,0).val[0]);
	d11 = fabs(VisionStatus::cvGet2D_11 - cvGet2D(SumWP,0,0).val[0]);
	d12 = fabs(VisionStatus::cvGet2D_12 - cvGet2D(SumWP,0,0).val[0]);

		//if((d1<d2)&&(d1<d3)&&(d1<d4)&&(d1<d5)&&(d1<d6)&&(d1<d7)&&(d1<d8)&&(d1<d9)&&(d1<d10)&&(d1<d11)&&(d1<d12))printf("Grandfa\n");
		//if((d7<d2)&&(d7<d3)&&(d7<d4)&&(d7<d5)&&(d7<d6)&&(d7<d1)&&(d7<d8)&&(d7<d9)&&(d7<d10)&&(d7<d11)&&(d7<d12))printf("Grandfa\n");
		//if((d2<d1)&&(d2<d3)&&(d2<d4)&&(d2<d5)&&(d2<d6)&&(d2<d7)&&(d2<d8)&&(d2<d9)&&(d2<d10)&&(d2<d11)&&(d2<d12))printf("Grandma\n");
		//if((d8<d2)&&(d8<d3)&&(d8<d4)&&(d8<d5)&&(d8<d6)&&(d8<d7)&&(d8<d1)&&(d8<d9)&&(d8<d10)&&(d8<d11)&&(d8<d12))printf("Grandma\n");
		//if((d3<d2)&&(d3<d1)&&(d3<d4)&&(d3<d5)&&(d3<d6)&&(d3<d7)&&(d3<d8)&&(d3<d9)&&(d3<d10)&&(d3<d11)&&(d3<d12))printf("Mother\n");
		//if((d9<d2)&&(d9<d3)&&(d9<d4)&&(d9<d5)&&(d9<d6)&&(d9<d7)&&(d9<d8)&&(d9<d1)&&(d9<d10)&&(d9<d11)&&(d9<d12))printf("Mother\n");
		//if((d4<d2)&&(d4<d3)&&(d4<d1)&&(d4<d5)&&(d4<d6)&&(d4<d7)&&(d4<d8)&&(d4<d9)&&(d4<d10)&&(d4<d11)&&(d4<d12))printf("Father\n");
		//if((d10<d2)&&(d10<d3)&&(d10<d4)&&(d10<d5)&&(d10<d6)&&(d10<d7)&&(d10<d8)&&(d10<d9)&&(d10<d1)&&(d10<d11)&&(d10<d12))printf("Father\n");
		//if((d5<d2)&&(d5<d3)&&(d5<d4)&&(d5<d1)&&(d5<d6)&&(d5<d7)&&(d5<d8)&&(d5<d9)&&(d5<d10)&&(d5<d11)&&(d5<d12))printf("Girl\n");
		//if((d11<d2)&&(d11<d3)&&(d11<d4)&&(d11<d5)&&(d11<d6)&&(d11<d7)&&(d11<d8)&&(d11<d9)&&(d11<d10)&&(d11<d1)&&(d11<d12))printf("Girl\n");
		//if((d6<d2)&&(d6<d3)&&(d6<d4)&&(d6<d5)&&(d6<d1)&&(d6<d7)&&(d6<d8)&&(d6<d9)&&(d6<d10)&&(d6<d11)&&(d6<d12))printf("Boy\n");
		//if((d12<d2)&&(d12<d3)&&(d12<d4)&&(d12<d5)&&(d12<d6)&&(d12<d7)&&(d12<d8)&&(d12<d9)&&(d12<d10)&&(d12<d11)&&(d12<d1))printf("Boy\n");
	}
}
