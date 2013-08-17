#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include "ImgProcess.h"
#include "VisionStatus.h"

using namespace std;
using namespace cv;
using namespace Robot;

String face_cascade_name = "../../../Data/jpg/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
CvMat *Vector1;
CvMat *AvgVector;
CvMat *EigenValue_Row;
CvMat *EigenVector;
CvMat *FFW;
CvMat *GfW;
CvMat *GmW;
CvMat *MW;
CvMat *FW;
CvMat *GW;
CvMat *BW;
CvMat *SumW1;
CvMat *SumW2;
CvMat *SumW3;
CvMat *SumW4;
CvMat *SumW5;
float Dall[40*40*5];
float GrandfaDW  [40*40];
float MotherDW  [40*40];
float FatherDW  [40*40];
float GirlDW  [40*40];
float BoyDW  [40*40];
void detection( Mat Image,Mat Face40size )
{
	int Facedata=0;
	std::vector<Rect> faces;
	Mat Image_gray;
	cvtColor( Image,Image_gray, CV_BGR2GRAY );
   	equalizeHist( Image_gray, Image_gray );
   	face_cascade.detectMultiScale( Image_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(40, 40) );
	Mat Image_Face( Size(faces[0].width,faces[0].height) , CV_8UC3 );
	for(int i=faces[0].y;i<faces[0].y + faces[0].height;i++){
		for(int j=faces[0].x;j<faces[0].x+faces[0].width;j++){
			Image_Face.data[Facedata] = Image.data[i*Image.cols*3+(j*3)];
			Image_Face.data[Facedata+1] = Image.data[i*Image.cols*3+(j*3)+1];
			Image_Face.data[Facedata+2] = Image.data[i*Image.cols*3+(j*3)+2];
			Facedata = Facedata+3;
		}
	}
	Mat Face_Gray;
	cvtColor( Image_Face, Face_Gray, CV_BGR2GRAY );
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Face40size.data[i*40*3+j*3] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
			Face40size.data[i*40*3+j*3+1] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
			Face40size.data[i*40*3+j*3+2] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
		}
	}
}
void Image_Unify( Mat Before , Mat After)
{
	int Iall=0,Iavg=0,I=0;
	for(int i=0;i<40*40;i++)Iall = Iall+Before.data[i*3];
	//Average gray
	Iavg = Iall/(40*40);
	//Unify brightness(128-Iavg)+Igary
	for(int i=0;i<40*40*3;i++){
		I = (128-Iavg)+Before.data[i];
		if(I<0)I=0;
		if(I>255)I=255;		
		After.data[i] = I;
	}
}
void differences_In_Face(Mat Before , Mat After , Mat Favg)
{
	int fd = 0;
	//The Grandfa's Differences In Face
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd = Before.data[i*40*3+j*3] - Favg.data[i*40*3+j*3];
			if(fd<0)fd=0;
			After.data[i*40*3+j*3]=fd;
			After.data[i*40*3+j*3+1]=fd;
			After.data[i*40*3+j*3+2]=fd;
		}
	}
}
void ImgProcess::FaceData()
{
	face_cascade.load( face_cascade_name );
	Mat Grandfa = imread( "../../../Data/jpg/Grandfa.jpg" , CV_LOAD_IMAGE_COLOR );
	Mat Mother = imread( "../../../Data/jpg/Mother.jpg" , CV_LOAD_IMAGE_COLOR );
	Mat Father = imread( "../../../Data/jpg/Father.jpg" , CV_LOAD_IMAGE_COLOR );
	Mat Girl = imread( "../../../Data/jpg/Girl.jpg" , CV_LOAD_IMAGE_COLOR );
	Mat Boy = imread( "../../../Data/jpg/Boy.jpg" , CV_LOAD_IMAGE_COLOR );
	Mat Grandfa_Face( Size(40,40) , CV_8UC3 );
	Mat Mother_Face( Size(40,40) , CV_8UC3 );
	Mat Father_Face( Size(40,40) , CV_8UC3 );
	Mat Girl_Face( Size(40,40) , CV_8UC3 );
	Mat Boy_Face( Size(40,40) , CV_8UC3 );
	detection( Grandfa,Grandfa_Face );
	detection( Mother,Mother_Face );
	detection( Father,Father_Face );
	detection( Girl,Girl_Face );
	detection( Boy,Boy_Face );
	Image_Unify( Grandfa_Face , Grandfa_Face);
	Image_Unify( Mother_Face , Mother_Face);
	Image_Unify( Father_Face , Father_Face);
	Image_Unify( Girl_Face , Girl_Face);
	Image_Unify( Boy_Face , Boy_Face);
	Mat Favg_Face( Size(40,40) , CV_8UC3 );
	for(int i=0;i<40*40*3;i++)
	Favg_Face.data[i] = ( Grandfa_Face.data[i]+Mother_Face.data[i]
			     +Father_Face.data[i]+Girl_Face.data[i]+Boy_Face.data[i] )/5;
	VisionStatus::Favg_Face = Favg_Face;
	differences_In_Face(Grandfa_Face , Grandfa_Face , Favg_Face);
	differences_In_Face(Mother_Face , Mother_Face , Favg_Face);
	differences_In_Face(Father_Face , Father_Face , Favg_Face);
	differences_In_Face(Girl_Face , Girl_Face , Favg_Face);
	differences_In_Face(Boy_Face , Boy_Face , Favg_Face);
	for (int i=0;i<40*40;i++){
		Dall[i*5+0]=Grandfa_Face.data[i*3];
		Dall[i*5+1]=Mother_Face.data[i*3];
		Dall[i*5+2]=Father_Face.data[i*3];
		Dall[i*5+3]=Girl_Face.data[i*3];
		Dall[i*5+4]=Boy_Face.data[i*3];
	}
	Vector1=cvCreateMat(40*40,5,CV_32FC1);
	cvSetData(Vector1,Dall,Vector1->step);
	AvgVector=cvCreateMat(1,5,CV_32FC1);
	EigenValue_Row=cvCreateMat(5,1,CV_32FC1);
	EigenVector=cvCreateMat(5,5,CV_32FC1);
	cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	int Num=0;
	float EigenValue_All=0,EigenValue_Max = cvGet2D(EigenValue_Row,0,0).val[0];
	for(int i=1;i<5;i++){
		EigenValue_All = cvGet2D(EigenValue_Row,i,0).val[0];
		if(EigenValue_All > EigenValue_Max){
			EigenValue_Max = EigenValue_All;
			Num=i;
		}
	}
	Mat Feature_Face( Size(40,40) , CV_8UC3 );
	int Feature_Sum=0;
	for(int i=0;i<40;i++){
		for(int j=0;j<40*3;j++){
			Feature_Sum = Grandfa_Face.data[i*40*3+j]*cvGet2D(EigenVector,Num,0).val[0]
			  	    + Mother_Face.data[i*40*3+j]*cvGet2D(EigenVector,Num,1).val[0]
			   	    + Father_Face.data[i*40*3+j]*cvGet2D(EigenVector,Num,2).val[0]
			   	    + Girl_Face.data[i*40*3+j]*cvGet2D(EigenVector,Num,3).val[0]
			   	    + Boy_Face.data[i*40*3+j]*cvGet2D(EigenVector,Num,4).val[0];
			if (Feature_Sum<0)Feature_Sum = 0;
			if (Feature_Sum>255)Feature_Sum = 255;
			Feature_Face.data[i*40*3+j] = Feature_Sum;
		}
	}
	for(int i=0;i<40*40;i++){
		VisionStatus::FeatureFaceW[i]=Feature_Face.data[i*3];
		GrandfaDW[i]=Grandfa_Face.data[i*3];
		MotherDW[i]=Mother_Face.data[i*3];
		FatherDW[i]=Father_Face.data[i*3];
		GirlDW[i]=Girl_Face.data[i*3];
		BoyDW[i]=Boy_Face.data[i*3];
	}
	FFW=cvCreateMat(1,40*40,CV_32FC1);
	cvSetData(FFW,VisionStatus::FeatureFaceW,FFW->step);
	GfW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(GfW,GrandfaDW,GfW->step);
	MW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(MW,MotherDW,MW->step);
	FW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(FW,FatherDW,FW->step);
	GW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(GW,GirlDW,GW->step);
	BW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(BW,BoyDW,BW->step);
	SumW1=cvCreateMat(1,1,CV_32FC1);
	SumW2=cvCreateMat(1,1,CV_32FC1);
	SumW3=cvCreateMat(1,1,CV_32FC1);
	SumW4=cvCreateMat(1,1,CV_32FC1);
	SumW5=cvCreateMat(1,1,CV_32FC1);
	cvMatMul(FFW,GfW,SumW1);
	cvMatMul(FFW,MW,SumW2);
	cvMatMul(FFW,FW,SumW3);
	cvMatMul(FFW,GW,SumW4);
	cvMatMul(FFW,BW,SumW5);
	printf("%f\n",cvGet2D(SumW1,0,0).val[0]);
	printf("%f\n",cvGet2D(SumW2,0,0).val[0]);
	printf("%f\n",cvGet2D(SumW3,0,0).val[0]);
	printf("%f\n",cvGet2D(SumW4,0,0).val[0]);
	printf("%f\n",cvGet2D(SumW5,0,0).val[0]); 
	
}


