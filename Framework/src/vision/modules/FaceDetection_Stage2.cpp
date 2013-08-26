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
        for(int a=faces[i].y; a<faces[i].y + faces[i].height; a++) {
            for(int j=faces[i].x; j<faces[i].x+faces[i].width; j++) {
                Face.data[Facedata] = Video.data[a*Video.cols*3+(j*3)];
                Face.data[Facedata+1] = Video.data[a*Video.cols*3+(j*3)+1];
                Face.data[Facedata+2] = Video.data[a*Video.cols*3+(j*3)+2];
                Facedata = Facedata+3;
            }
        }
        Mat Face_Gray;
        cvtColor( Face, Face_Gray, CV_BGR2GRAY );
        for(int a=0; a<40; a++) {
            for(int j=0; j<40; j++) {
                Face40size.data[a*40*3+j*3] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
                Face40size.data[a*40*3+j*3+1] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
                Face40size.data[a*40*3+j*3+2] = Face_Gray.data[(a*Face.rows/40)*Face.cols+(j*Face.cols/40)];
            }
        }
        Iall=0;
        for(int a=0; a<40*40; a++)Iall = Iall+Face40size.data[a*3];
        Iavg = Iall/(40*40);
        for(int a=0; a<40*40*3; a++) {
            I = (128-Iavg)+Face40size.data[a];
            if(I<0)I=0;
            if(I>255)I=255;
            Face40size.data[a] = I;
        }
        int fd = 0;
        if(faces[i].width >= 100) {
            for(int a=0; a<40; a++) {
                for(int j=0; j<40; j++) {
                    fd = Face40size.data[a*40*3+j*3] - VisionStatus::Favg_Face_20cm.data[a*40*3+j*3];
                    if(fd<0)fd=0;
                    Face40size.data[a*40*3+j*3]=fd;
                    Face40size.data[a*40*3+j*3+1]=fd;
                    Face40size.data[a*40*3+j*3+2]=fd;
                }
            }
        } else {
            for(int a=0; a<40; a++) {
                for(int j=0; j<40; j++) {
                    fd = Face40size.data[a*40*3+j*3] - VisionStatus::Favg_Face_50cm.data[a*40*3+j*3];
                    if(fd<0)fd=0;
                    Face40size.data[a*40*3+j*3]=fd;
                    Face40size.data[a*40*3+j*3+1]=fd;
                    Face40size.data[a*40*3+j*3+2]=fd;
                }
            }

        }
        for(int a=0; a<40*40; a++)PeopleDW[a]=Face40size.data[a*3];
        if(faces[i].width >= 100) {
            FFW=cvCreateMat(1,40*40,CV_32FC1);
            cvSetData(FFW,VisionStatus::FeatureFaceW_20cm,FFW->step);
        } else {
            FFW=cvCreateMat(1,40*40,CV_32FC1);
            cvSetData(FFW,VisionStatus::FeatureFaceW_50cm,FFW->step);
        }
        PW=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(PW,PeopleDW,PW->step);
        SumWP=cvCreateMat(1,1,CV_32FC1);
        cvMatMul(FFW,PW,SumWP);
        if( faces[i].width >= 100 )
        {
            //		printf("%d\n",faces[i].width);
//		printf("%f\n",cvGet2D(SumWP,0,0).val[0]);
            if(( 10000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<19000 ) {
//			printf("Grandfa\n");
                VisionStatus::Grandfa_counter++;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 180000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<400000 ) {
//			printf("Mother\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter++;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 400000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<540000 ) {
//			printf("Father\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter++;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 30000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<56500 ) {
//			printf("Girl\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter++;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 70000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<140000 ) {
//			printf("Boy\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter++;
                VisionStatus::Thief_counter=0;
            }
            else {
//			printf("Niss\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter++;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter++;
            }
        } else {
            //	printf("%d\n",faces[i].width);
            //	printf("%f\n",cvGet2D(SumWP,0,0).val[0]);
            if(( 18000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<50000 ) {
                //		printf("Grandfa\n");
                VisionStatus::Grandfa_counter++;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 190000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<330000 ) {
                //		printf("Mother\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter++;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 330000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<530000 ) {
                //		printf("Father\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter++;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 50000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<70000 ) {
                //		printf("Girl\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter++;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter=0;
            }
            else if(( 70000<cvGet2D(SumWP,0,0).val[0] )&&( cvGet2D(SumWP,0,0).val[0])<130000 ) {
                //		printf("Boy\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter=0;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter++;
                VisionStatus::Thief_counter=0;
            }
            else {
                //		printf("Miss\n");
                VisionStatus::Grandfa_counter=0;
                VisionStatus::Grandma_counter++;
                VisionStatus::Mother_counter=0;
                VisionStatus::Father_counter=0;
                VisionStatus::Girl_counter=0;
                VisionStatus::Boy_counter=0;
                VisionStatus::Thief_counter++;
            }
        }
        StrategyStatus::FlagMember = true;
        /*if( VisionStatus::Grandfa_counter== GET_FACE_CNT) {
            //printf("Grandfa\n");
            StrategyStatus::FamilyMember = 0;
        }
        if( VisionStatus::Grandma_counter==GET_FACE_CNT) {
            //printf("Grandma\n");
            StrategyStatus::FamilyMember = 1;
        }
        if( VisionStatus::Mother_counter== GET_FACE_CNT) {
            //printf("Mother\n");
            StrategyStatus::FamilyMember = 3;
        }
        if( VisionStatus::Father_counter==GET_FACE_CNT) {
            //printf("Father\n");
            StrategyStatus::FamilyMember = 2;
        }
        if( VisionStatus::Girl_counter== GET_FACE_CNT) {
            //printf("Girl\n");
            StrategyStatus::FamilyMember = 5;
        }
        if( VisionStatus::Boy_counter== GET_FACE_CNT) {
            //printf("Boy\n");
            StrategyStatus::FamilyMember = 4;
        }*/
        if(VisionStatus::Thief_counter== GET_FACE_CNT){
		/*printf("Thief\n");*/ 
		StrategyStatus::FlagThief = true;
		cv::imwrite("Thief.jpg",VisionStatus::VideoFrame);
	}
    }
}
