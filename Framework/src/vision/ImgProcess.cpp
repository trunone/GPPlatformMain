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

unsigned char Buffer1[800*600*3];
unsigned char Grandfas[40*40*3];
unsigned char Grandmas[40*40*3];
unsigned char Mothers[40*40*3];
unsigned char Fathers[40*40*3];
unsigned char Girls[40*40*3];
unsigned char Boys[40*40*3];
unsigned char Grandfas1[40*40*3];
unsigned char Grandmas1[40*40*3];
unsigned char Mothers1[40*40*3];
unsigned char Fathers1[40*40*3];
unsigned char Girls1[40*40*3];
unsigned char Boys1[40*40*3];
unsigned char GrandfaD[40*40*3];
unsigned char GrandmaD[40*40*3];
unsigned char MotherD[40*40*3];
unsigned char FatherD[40*40*3];
unsigned char GirlD[40*40*3];
unsigned char BoyD[40*40*3];
unsigned char Grandfa1D[40*40*3];
unsigned char Grandma1D[40*40*3];
unsigned char Mother1D[40*40*3];
unsigned char Father1D[40*40*3];
unsigned char Girl1D[40*40*3];
unsigned char Boy1D[40*40*3];
float Dall[40*40*12];
float FeatureFace[40*40*3];
float GrandfaDW  [40*40];
float GrandmaDW  [40*40];
float MotherDW  [40*40];
float FatherDW  [40*40];
float GirlDW  [40*40];
float BoyDW  [40*40];
float Grandfa1DW  [40*40];
float Grandma1DW  [40*40];
float Mother1DW  [40*40];
float Father1DW  [40*40];
float Girl1DW  [40*40];
float Boy1DW  [40*40];
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
CvMat *Gf1W;
CvMat *Gm1W;
CvMat *M1W;
CvMat *F1W;
CvMat *G1W;
CvMat *B1W;
CvMat *SumW1;
CvMat *SumW2;
CvMat *SumW3;
CvMat *SumW4;
CvMat *SumW5;
CvMat *SumW6;
CvMat *SumW7;
CvMat *SumW8;
CvMat *SumW9;
CvMat *SumW10;
CvMat *SumW11;
CvMat *SumW12;

void Image_Unify(unsigned char *Before , unsigned char *After)
{
	float Iall=0,Igray=0,Iavg=0,I=0,Ih=0;
	//Gray
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Before[i*40*3+j*3]
			+ 0.587*Before[i*40*3+j*3+1]
			+ 0.299*Before[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//Average gray
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//Unify brightness(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Before[i*40*3+j*3]
			+ 0.587*Before[i*40*3+j*3+1]
			+ 0.299*Before[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			After[i*40*3+j*3]=Ih;
			After[i*40*3+j*3+1]=Ih;
			After[i*40*3+j*3+2]=Ih;
		}
	}
}

void Differences_In_Face(unsigned char *Before , unsigned char *After , unsigned char *Favg)
{
	float fd = 0;
	//The Grandfa's Differences In Face
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd = Before[i*40*3+j*3] - Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			After[i*40*3+j*3]=fd;
			After[i*40*3+j*3+1]=fd;
			After[i*40*3+j*3+2]=fd;
		}
	}
}

void ImgProcess::FaceData()
{
	int b=0,w=0,h=0,ui=0;
	int Dalls=0;
	float favg=0,cp0=0,cp1=0,ff=0;
	IplImage Grandfa=*cvLoadImage("../../../Data/jpg/Grandfa.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandma=*cvLoadImage("../../../Data/jpg/Grandma.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Mother=*cvLoadImage("../../../Data/jpg/Mother.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Father=*cvLoadImage("../../../Data/jpg/Father.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Girl=*cvLoadImage("../../../Data/jpg/Girl.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Boy=*cvLoadImage("../../../Data/jpg/Boy.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandfa1=*cvLoadImage("../../../Data/jpg/Grandfa1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandma1=*cvLoadImage("../../../Data/jpg/Grandma1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Mother1=*cvLoadImage("../../../Data/jpg/Mother1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Father1=*cvLoadImage("../../../Data/jpg/Father1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Girl1=*cvLoadImage("../../../Data/jpg/Girl1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Boy1=*cvLoadImage("../../../Data/jpg/Boy1.jpg", CV_LOAD_IMAGE_COLOR);
	/////////////////////////////////////////Grandfa face////////////////////////////////////////////////////////////////
	b=0;
 	//Cut out the face
	for(int a=265;a<265 + 49;a++){
		for(int j=474;j<474 + 49;j++){
			Buffer1[b] = Grandfa.imageData[a*Grandfa.widthStep+j*3];
			Buffer1[b+1] = Grandfa.imageData[a*Grandfa.widthStep+j*3+1];
			Buffer1[b+2] = Grandfa.imageData[a*Grandfa.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandfas[i*40*3+j*3]=Buffer1[(i*49/40)*49*3+(j*49/40)*3];
			Grandfas[i*40*3+j*3+1]=Buffer1[(i*49/40)*49*3+(j*49/40)*3+1];
			Grandfas[i*40*3+j*3+2]=Buffer1[(i*49/40)*49*3+(j*49/40)*3+2];
		}
	}
	Image_Unify(Grandfas,Grandfas);
	/////////////////////////////////////////Grandma face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=196;a<196 + 92;a++){
		for(int j=445;j<445 + 92;j++){
			Buffer1[b] = Grandma.imageData[a*Grandma.widthStep+j*3];
			Buffer1[b+1] = Grandma.imageData[a*Grandma.widthStep+j*3+1];
			Buffer1[b+2] = Grandma.imageData[a*Grandma.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandmas[i*40*3+j*3]=Buffer1[(i*92/40)*92*3+(j*92/40)*3];
			Grandmas[i*40*3+j*3+1]=Buffer1[(i*92/40)*92*3+(j*92/40)*3+1];
			Grandmas[i*40*3+j*3+2]=Buffer1[(i*92/40)*92*3+(j*92/40)*3+2];
		}
	}
	Image_Unify(Grandmas,Grandmas);
	//////////////////////////////////Mother face//////////////////////////////////////////////////////////////////				 
	b=0;
	//Cut out the face
	for(int a=257;a<257 + 70;a++){
		for(int j=434;j<434 + 70;j++){
			Buffer1[b] = Mother.imageData[a*Mother.widthStep+j*3];
			Buffer1[b+1] = Mother.imageData[a*Mother.widthStep+j*3+1];
			Buffer1[b+2] = Mother.imageData[a*Mother.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Mothers[i*40*3+j*3]=Buffer1[(i*70/40)*70*3+(j*70/40)*3];
			Mothers[i*40*3+j*3+1]=Buffer1[(i*70/40)*70*3+(j*70/40)*3+1];
			Mothers[i*40*3+j*3+2]=Buffer1[(i*70/40)*70*3+(j*70/40)*3+2];
		}
	}
	Image_Unify(Mothers,Mothers);
	//////////////////////////////////////////Father face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=265;a<265 + 53;a++){
		for(int j=455;j<455 + 53;j++){
			Buffer1[b] = Father.imageData[a*Father.widthStep+j*3];
			Buffer1[b+1] = Father.imageData[a*Father.widthStep+j*3+1];
			Buffer1[b+2] = Father.imageData[a*Father.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Fathers[i*40*3+j*3]=Buffer1[(i*53/40)*53*3+(j*53/40)*3];
			Fathers[i*40*3+j*3+1]=Buffer1[(i*53/40)*53*3+(j*53/40)*3+1];
			Fathers[i*40*3+j*3+2]=Buffer1[(i*53/40)*53*3+(j*53/40)*3+2];
		}
	}
	Image_Unify(Fathers,Fathers);
	/////////////////////////////////////////Girl face//////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=269;a<269 + 69;a++){
		for(int j=472;j<472 + 69;j++){
			Buffer1[b] = Girl.imageData[a*Girl.widthStep+j*3];
			Buffer1[b+1] = Girl.imageData[a*Girl.widthStep+j*3+1];
			Buffer1[b+2] = Girl.imageData[a*Girl.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Girls[i*40*3+j*3]=Buffer1[(i*69/40)*69*3+(j*69/40)*3];
			Girls[i*40*3+j*3+1]=Buffer1[(i*69/40)*69*3+(j*69/40)*3+1];
			Girls[i*40*3+j*3+2]=Buffer1[(i*69/40)*69*3+(j*69/40)*3+2];
		}
	}
	Image_Unify(Girls,Girls);
	/////////////////////////////////////////Boy face///////////////////////////////////////////////////////////////////// 
	b=0;
	//Cut out the face
	for(int a=265;a<265 + 65;a++){
		for(int j=429;j<429 + 65;j++){
			Buffer1[b] = Boy.imageData[a*Boy.widthStep+j*3];
			Buffer1[b+1] = Boy.imageData[a*Boy.widthStep+j*3+1];
			Buffer1[b+2] = Boy.imageData[a*Boy.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Boys[i*40*3+j*3]=Buffer1[(i*65/40)*65*3+(j*65/40)*3];
			Boys[i*40*3+j*3+1]=Buffer1[(i*65/40)*65*3+(j*65/40)*3+1];
			Boys[i*40*3+j*3+2]=Buffer1[(i*65/40)*65*3+(j*65/40)*3+2];
		}
	}
	Image_Unify(Boys,Boys);
	/////////////////////////////////////////Grandfa1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=344;a<344 + 52;a++){
		for(int j=385;j<385 + 52;j++){
			Buffer1[b] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3];
			Buffer1[b+1] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3+1];
			Buffer1[b+2] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandfas1[i*40*3+j*3]=Buffer1[(i*52/40)*52*3+(j*52/40)*3];
			Grandfas1[i*40*3+j*3+1]=Buffer1[(i*52/40)*52*3+(j*52/40)*3+1];
			Grandfas1[i*40*3+j*3+2]=Buffer1[(i*52/40)*52*3+(j*52/40)*3+2];
		}
	}
	Image_Unify(Grandfas1,Grandfas1);
	/////////////////////////////////////////Grandma1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=419;a<419 + 64;a++){
		for(int j=364;j<364 + 64;j++){
			Buffer1[b] = Grandma1.imageData[a*Grandma1.widthStep+j*3];
			Buffer1[b+1] = Grandma1.imageData[a*Grandma1.widthStep+j*3+1];
			Buffer1[b+2] = Grandma1.imageData[a*Grandma1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandmas1[i*40*3+j*3]=Buffer1[(i*64/40)*64*3+(j*64/40)*3];
			Grandmas1[i*40*3+j*3+1]=Buffer1[(i*64/40)*64*3+(j*64/40)*3+1];
			Grandmas1[i*40*3+j*3+2]=Buffer1[(i*64/40)*64*3+(j*64/40)*3+2];
		}
	}
	Image_Unify(Grandmas1,Grandmas1);
	/////////////////////////////////////////Mother1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=355;a<355 + 76;a++){
		for(int j=362;j<362 + 76;j++){
			Buffer1[b] = Mother1.imageData[a*Mother1.widthStep+j*3];
			Buffer1[b+1] = Mother1.imageData[a*Mother1.widthStep+j*3+1];
			Buffer1[b+2] = Mother1.imageData[a*Mother1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Mothers1[i*40*3+j*3]=Buffer1[(i*76/40)*76*3+(j*76/40)*3];
			Mothers1[i*40*3+j*3+1]=Buffer1[(i*76/40)*76*3+(j*76/40)*3+1];
			Mothers1[i*40*3+j*3+2]=Buffer1[(i*76/40)*76*3+(j*76/40)*3+2];
		}
	}
	Image_Unify(Mothers1,Mothers1);
	/////////////////////////////////////////Father1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=407;a<407 + 57;a++){
		for(int j=362;j<362 + 57;j++){
			Buffer1[b] = Father1.imageData[a*Father1.widthStep+j*3];
			Buffer1[b+1] = Father1.imageData[a*Father1.widthStep+j*3+1];
			Buffer1[b+2] = Father1.imageData[a*Father1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Fathers1[i*40*3+j*3]=Buffer1[(i*57/40)*57*3+(j*57/40)*3];
			Fathers1[i*40*3+j*3+1]=Buffer1[(i*57/40)*57*3+(j*57/40)*3+1];
			Fathers1[i*40*3+j*3+2]=Buffer1[(i*57/40)*57*3+(j*57/40)*3+2];
		}
	}
	Image_Unify(Fathers1,Fathers1);
	/////////////////////////////////////////Girl1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=408;a<408 + 82;a++){
		for(int j=388;j<388 + 82;j++){
			Buffer1[b] = Girl1.imageData[a*Girl1.widthStep+j*3];
			Buffer1[b+1] = Girl1.imageData[a*Girl1.widthStep+j*3+1];
			Buffer1[b+2] = Girl1.imageData[a*Girl1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Girls1[i*40*3+j*3]=Buffer1[(i*82/40)*82*3+(j*82/40)*3];
			Girls1[i*40*3+j*3+1]=Buffer1[(i*82/40)*82*3+(j*82/40)*3+1];
			Girls1[i*40*3+j*3+2]=Buffer1[(i*82/40)*82*3+(j*82/40)*3+2];
		}
	}
	Image_Unify(Girls1,Girls1);
	/////////////////////////////////////////Boy1 face////////////////////////////////////////////////////////////////
	b=0;
	//Cut out the face
	for(int a=412;a<412 + 77;a++){
		for(int j=330;j<330 + 77;j++){
			Buffer1[b] = Boy1.imageData[a*Boy1.widthStep+j*3];
			Buffer1[b+1] = Boy1.imageData[a*Boy1.widthStep+j*3+1];
			Buffer1[b+2] = Boy1.imageData[a*Boy1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//Minify40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Boys1[i*40*3+j*3]=Buffer1[(i*77/40)*77*3+(j*77/40)*3];
			Boys1[i*40*3+j*3+1]=Buffer1[(i*77/40)*77*3+(j*77/40)*3+1];
			Boys1[i*40*3+j*3+2]=Buffer1[(i*77/40)*77*3+(j*77/40)*3+2];
		}
	}
	Image_Unify(Boys1,Boys1);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//The Average Face
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			favg = ( Grandfas[i*40*3+j*3] + Grandmas[i*40*3+j*3]
			       + Mothers[i*40*3+j*3] + Fathers[i*40*3+j*3]
		               + Girls[i*40*3+j*3] + Boys[i*40*3+j*3]
			       + Grandfas1[i*40*3+j*3] + Grandmas1[i*40*3+j*3]
			       + Mothers1[i*40*3+j*3] + Fathers1[i*40*3+j*3]
			       + Girls1[i*40*3+j*3] + Boys1[i*40*3+j*3] ) / 12;
			VisionStatus::Favg[i*40*3+j*3]=favg;
			VisionStatus::Favg[i*40*3+j*3+1]=favg;
			VisionStatus::Favg[i*40*3+j*3+2]=favg;
		}
	}
	Differences_In_Face(Grandfas , GrandfaD , VisionStatus::Favg);
	Differences_In_Face(Grandmas , GrandmaD , VisionStatus::Favg);
	Differences_In_Face(Mothers , MotherD , VisionStatus::Favg);
	Differences_In_Face(Fathers , FatherD , VisionStatus::Favg);
	Differences_In_Face(Girls , GirlD , VisionStatus::Favg);
	Differences_In_Face(Boys , BoyD , VisionStatus::Favg);
	Differences_In_Face(Grandfas1 , Grandfa1D , VisionStatus::Favg);
	Differences_In_Face(Grandmas1 , Grandma1D , VisionStatus::Favg);
	Differences_In_Face(Mothers1 , Mother1D , VisionStatus::Favg);
	Differences_In_Face(Fathers1 , Father1D , VisionStatus::Favg);
	Differences_In_Face(Girls1 , Girl1D , VisionStatus::Favg);
	Differences_In_Face(Boys1 , Boy1D , VisionStatus::Favg);
	//All Different Face Put In The Dall's Array
	for (int i=0;i<40*40;i++){
		Dalls=i*12;
		Dall[Dalls]=GrandfaD[i*3];
		Dall[Dalls+1]=GrandmaD[i*3];
		Dall[Dalls+2]=MotherD[i*3];
		Dall[Dalls+3]=FatherD[i*3];
		Dall[Dalls+4]=GirlD[i*3];
		Dall[Dalls+5]=BoyD[i*3];
		Dall[Dalls+6]=Grandfa1D[i*3];
		Dall[Dalls+7]=Grandma1D[i*3];
		Dall[Dalls+8]=Mother1D[i*3];
		Dall[Dalls+9]=Father1D[i*3];
		Dall[Dalls+10]=Girl1D[i*3];
		Dall[Dalls+11]=Boy1D[i*3];
	}
	//The Dall's Array turn into One-Dimensional Array
	Vector1=cvCreateMat(40*40,12,CV_32FC1);
	cvSetData(Vector1,Dall,Vector1->step);
	//AvgVector
	AvgVector=cvCreateMat(1,12,CV_32FC1);
	//EigenValue
	EigenValue_Row=cvCreateMat(12,1,CV_32FC1);
	//EigenVector
	EigenVector=cvCreateMat(12,12,CV_32FC1);
	//PCA(Principal Comonents Analysis)Calculate
	cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	//Search For Maximum  EigenValue
	cp0=cvGet2D(EigenValue_Row,0,0).val[0];
	ui=0;
	for(int i=1;i<12;i++){
		cp1=cvGet2D(EigenValue_Row,i,0).val[0];
		if(cp1>cp0){
			cp0=cp1;
			ui=i;
		}
	}
	//The Eigenface
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			ff=GrandfaD[i*40*3+j*3]*cvGet2D(EigenVector,ui,0).val[0]
			+GrandmaD[i*40*3+j*3]*cvGet2D(EigenVector,ui,1).val[0]
			+MotherD[i*40*3+j*3]*cvGet2D(EigenVector,ui,2).val[0]
			+FatherD[i*40*3+j*3]*cvGet2D(EigenVector,ui,3).val[0]
			+GirlD[i*40*3+j*3]*cvGet2D(EigenVector,ui,4).val[0]
			+BoyD[i*40*3+j*3]*cvGet2D(EigenVector,ui,5).val[0]
			+Grandfa1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,6).val[0]
			+Grandma1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,7).val[0]
			+Mother1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,8).val[0]
			+Father1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,9).val[0]
			+Girl1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,10).val[0]
			+Boy1D[i*40*3+j*3]*cvGet2D(EigenVector,ui,11).val[0];
			if (ff<0)ff=0;
			if (ff>255)ff=255;
			FeatureFace[i*40*3+(j*3)]=ff;
			FeatureFace[i*40*3+(j*3)+1]=ff;
			FeatureFace[i*40*3+(j*3)+2]=ff;
		}
	}
	//Differences In Face Turn Into Float Type 
	for(int i=0;i<40*40;i++){
		VisionStatus::FeatureFaceW[i]=FeatureFace[i*3];
		GrandfaDW[i]=GrandfaD[i*3];
		GrandmaDW[i]=GrandmaD[i*3];
		MotherDW[i]=MotherD[i*3];
		FatherDW[i]=FatherD[i*3];
		GirlDW[i]=GirlD[i*3];
		BoyDW[i]=BoyD[i*3];
		Grandfa1DW[i]=Grandfa1D[i*3];
		Grandma1DW[i]=Grandma1D[i*3];
		Mother1DW[i]=Mother1D[i*3];
		Father1DW[i]=Father1D[i*3];
		Girl1DW[i]=Girl1D[i*3];
		Boy1DW[i]=Boy1D[i*3];
	}
	//The Eigenface turn into One-Dimensional Array
	FFW=cvCreateMat(1,40*40,CV_32FC1);
	cvSetData(FFW,VisionStatus::FeatureFaceW,FFW->step);
	//Differences In Face Turn Into One-Dimensional Array
	GfW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(GfW,GrandfaDW,GfW->step);
	GmW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(GmW,GrandmaDW,GmW->step);
	MW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(MW,MotherDW,MW->step);
	FW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(FW,FatherDW,FW->step);
	GW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(GW,GirlDW,GW->step);
	BW=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(BW,BoyDW,BW->step);
	Gf1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(Gf1W,Grandfa1DW,Gf1W->step);
	Gm1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(Gm1W,Grandma1DW,Gm1W->step);
	M1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(M1W,Mother1DW,M1W->step);
	F1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(F1W,Father1DW,F1W->step);
	G1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(G1W,Girl1DW,G1W->step);
	B1W=cvCreateMat(40*40,1,CV_32FC1);
	cvSetData(B1W,Boy1DW,B1W->step);
	SumW1=cvCreateMat(1,1,CV_32FC1);
	SumW2=cvCreateMat(1,1,CV_32FC1);
	SumW3=cvCreateMat(1,1,CV_32FC1);
	SumW4=cvCreateMat(1,1,CV_32FC1);
	SumW5=cvCreateMat(1,1,CV_32FC1);
	SumW6=cvCreateMat(1,1,CV_32FC1);
	SumW7=cvCreateMat(1,1,CV_32FC1);
	SumW8=cvCreateMat(1,1,CV_32FC1);
	SumW9=cvCreateMat(1,1,CV_32FC1);
	SumW10=cvCreateMat(1,1,CV_32FC1);
	SumW11=cvCreateMat(1,1,CV_32FC1);
	SumW12=cvCreateMat(1,1,CV_32FC1);
	//­Differences In Face Project to The Eigenface
	cvMatMul(FFW,GfW,SumW1);
	cvMatMul(FFW,GmW,SumW2);
	cvMatMul(FFW,MW,SumW3);
	cvMatMul(FFW,FW,SumW4);
	cvMatMul(FFW,GW,SumW5);
	cvMatMul(FFW,BW,SumW6);
	cvMatMul(FFW,Gf1W,SumW7);
	cvMatMul(FFW,Gm1W,SumW8);
	cvMatMul(FFW,M1W,SumW9);
	cvMatMul(FFW,F1W,SumW10);
	cvMatMul(FFW,G1W,SumW11);
	cvMatMul(FFW,B1W,SumW12);
	//printf("%f\n",cvGet2D(SumW1,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW2,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW3,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW4,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW5,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW6,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW7,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW8,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW9,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW10,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW11,0,0).val[0]);
	//printf("%f\n",cvGet2D(SumW12,0,0).val[0]); 
}


