#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include "VisionStatus.h"

using namespace std;
using namespace cv;
using namespace Robot;

	string face_cascade_name = "haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;
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
	float FeatureFaceW  [40*40];
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

void FaceData()
{
	int b=0,w=0,h=0,ui=0;
	int Dalls=0;
	float Iall=0,Igray=0,Iavg=0,I=0,Ih=0,favg=0,fd=0,cp0=0,cp1=0,ff=0;
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n");};
	IplImage Grandfa=*cvLoadImage("Grandfa.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandma=*cvLoadImage("Grandma.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Mother=*cvLoadImage("Mother.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Father=*cvLoadImage("Father.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Girl=*cvLoadImage("Girl.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Boy=*cvLoadImage("Boy.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandfa1=*cvLoadImage("Grandfa1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Grandma1=*cvLoadImage("Grandma1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Mother1=*cvLoadImage("Mother1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Father1=*cvLoadImage("Father1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Girl1=*cvLoadImage("Girl1.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage Boy1=*cvLoadImage("Boy1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat frame1(&Grandfa, 0);
	Mat frame2(&Grandma, 0);
	Mat frame3(&Mother, 0);
	Mat frame4(&Father, 0);
	Mat frame5(&Girl, 0);
	Mat frame6(&Boy, 0);
	Mat frame7(&Grandfa1, 0);
	Mat frame8(&Grandma1, 0);
	Mat frame9(&Mother1, 0);
	Mat frame10(&Father1, 0);
	Mat frame11(&Girl1, 0);
	Mat frame12(&Boy1, 0);
	Mat frame_gray;
	Mat frame_gray1;
	Mat frame_gray2;
	Mat frame_gray3;
	Mat frame_gray4;
	Mat frame_gray5;
	Mat frame_gray6;
	Mat frame_gray7;
	Mat frame_gray8;
	Mat frame_gray9;
	Mat frame_gray10;
	Mat frame_gray11;
	Mat frame_gray12;
	std::vector<Rect> faces;
	cvtColor( frame1, frame_gray1, CV_BGR2GRAY );
	cvtColor( frame2, frame_gray2, CV_BGR2GRAY );
	cvtColor( frame3, frame_gray3, CV_BGR2GRAY );
	cvtColor( frame4, frame_gray4, CV_BGR2GRAY );
	cvtColor( frame5, frame_gray5, CV_BGR2GRAY );
	cvtColor( frame6, frame_gray6, CV_BGR2GRAY );
	cvtColor( frame7, frame_gray7, CV_BGR2GRAY );
	cvtColor( frame8, frame_gray8, CV_BGR2GRAY );
	cvtColor( frame9, frame_gray9, CV_BGR2GRAY );
	cvtColor( frame10, frame_gray10, CV_BGR2GRAY );
	cvtColor( frame11, frame_gray11, CV_BGR2GRAY );
	cvtColor( frame12, frame_gray12, CV_BGR2GRAY );
	equalizeHist( frame_gray1, frame_gray1 );
	equalizeHist( frame_gray2, frame_gray2 );
	equalizeHist( frame_gray3, frame_gray3 );
	equalizeHist( frame_gray4, frame_gray4 );
	equalizeHist( frame_gray5, frame_gray5 );
	equalizeHist( frame_gray6, frame_gray6 );
	equalizeHist( frame_gray7, frame_gray7 );
	equalizeHist( frame_gray8, frame_gray8 );
	equalizeHist( frame_gray9, frame_gray9 );
	equalizeHist( frame_gray10, frame_gray10 );
	equalizeHist( frame_gray11, frame_gray11 );
	equalizeHist( frame_gray12, frame_gray12 );
	/////////////////////////////////////////GrandfaŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray1, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Grandfa.imageData[a*Grandfa.widthStep+j*3];
			Buffer1[b+1] = Grandfa.imageData[a*Grandfa.widthStep+j*3+1];
			Buffer1[b+2] = Grandfa.imageData[a*Grandfa.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandfas[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Grandfas[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Grandfas[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandfas[i*40*3+j*3]
			+ 0.587*Grandfas[i*40*3+j*3+1]
			+ 0.299*Grandfas[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandfas[i*40*3+j*3]
			+ 0.587*Grandfas[i*40*3+j*3+1]
			+ 0.299*Grandfas[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Grandfas[i*40*3+j*3]=Ih;
			Grandfas[i*40*3+j*3+1]=Ih;
			Grandfas[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////GrandmaŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray2, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40,40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Grandma.imageData[a*Grandma.widthStep+j*3];
			Buffer1[b+1] = Grandma.imageData[a*Grandma.widthStep+j*3+1];
			Buffer1[b+2] = Grandma.imageData[a*Grandma.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandmas[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Grandmas[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Grandmas[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandmas[i*40*3+j*3]
			+ 0.587*Grandmas[i*40*3+j*3+1]
			+ 0.299*Grandmas[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandmas[i*40*3+j*3]
			+ 0.587*Grandmas[i*40*3+j*3+1]
			+ 0.299*Grandmas[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Grandmas[i*40*3+j*3]=Ih;
			Grandmas[i*40*3+j*3+1]=Ih;
			Grandmas[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////MotherŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				 
	face_cascade.detectMultiScale( frame_gray3, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Mother.imageData[a*Mother.widthStep+j*3];
			Buffer1[b+1] = Mother.imageData[a*Mother.widthStep+j*3+1];
			Buffer1[b+2] = Mother.imageData[a*Mother.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Mothers[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Mothers[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Mothers[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Mothers[i*40*3+j*3]
			+ 0.587*Mothers[i*40*3+j*3+1]
			+ 0.299*Mothers[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Mothers[i*40*3+j*3]
			+ 0.587*Mothers[i*40*3+j*3+1]
			+ 0.299*Mothers[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Mothers[i*40*3+j*3]=Ih;
			Mothers[i*40*3+j*3+1]=Ih;
			Mothers[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////FatherŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray4, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Father.imageData[a*Father.widthStep+j*3];
			Buffer1[b+1] = Father.imageData[a*Father.widthStep+j*3+1];
			Buffer1[b+2] = Father.imageData[a*Father.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Fathers[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Fathers[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Fathers[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Fathers[i*40*3+j*3]
			+ 0.587*Fathers[i*40*3+j*3+1]
			+ 0.299*Fathers[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Fathers[i*40*3+j*3]
			+ 0.587*Fathers[i*40*3+j*3+1]
			+ 0.299*Fathers[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Fathers[i*40*3+j*3]=Ih;
			Fathers[i*40*3+j*3+1]=Ih;
			Fathers[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////GirlŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				
	face_cascade.detectMultiScale( frame_gray5, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Girl.imageData[a*Girl.widthStep+j*3];
			Buffer1[b+1] = Girl.imageData[a*Girl.widthStep+j*3+1];
			Buffer1[b+2] = Girl.imageData[a*Girl.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Girls[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Girls[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Girls[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Girls[i*40*3+j*3]
			+ 0.587*Girls[i*40*3+j*3+1]
			+ 0.299*Girls[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Girls[i*40*3+j*3]
			+ 0.587*Girls[i*40*3+j*3+1]
			+ 0.299*Girls[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Girls[i*40*3+j*3]=Ih;
			Girls[i*40*3+j*3+1]=Ih;
			Girls[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////BoyŒv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				 
	face_cascade.detectMultiScale( frame_gray6, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Boy.imageData[a*Boy.widthStep+j*3];
			Buffer1[b+1] = Boy.imageData[a*Boy.widthStep+j*3+1];
			Buffer1[b+2] = Boy.imageData[a*Boy.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Boys[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Boys[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Boys[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Boys[i*40*3+j*3]
			+ 0.587*Boys[i*40*3+j*3+1]
			+ 0.299*Boys[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Boys[i*40*3+j*3]
			+ 0.587*Boys[i*40*3+j*3+1]
			+ 0.299*Boys[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Boys[i*40*3+j*3]=Ih;
			Boys[i*40*3+j*3+1]=Ih;
			Boys[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Grandfa1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray7, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3];
			Buffer1[b+1] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3+1];
			Buffer1[b+2] = Grandfa1.imageData[a*Grandfa1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandfas1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Grandfas1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Grandfas1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandfas1[i*40*3+j*3]
			+ 0.587*Grandfas1[i*40*3+j*3+1]
			+ 0.299*Grandfas1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandfas1[i*40*3+j*3]
			+ 0.587*Grandfas1[i*40*3+j*3+1]
			+ 0.299*Grandfas1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Grandfas1[i*40*3+j*3]=Ih;
			Grandfas1[i*40*3+j*3+1]=Ih;
			Grandfas1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Grandma1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray8, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40,40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Grandma1.imageData[a*Grandma1.widthStep+j*3];
			Buffer1[b+1] = Grandma1.imageData[a*Grandma1.widthStep+j*3+1];
			Buffer1[b+2] = Grandma1.imageData[a*Grandma1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Grandmas1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Grandmas1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Grandmas1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandmas1[i*40*3+j*3]
			+ 0.587*Grandmas1[i*40*3+j*3+1]
			+ 0.299*Grandmas1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Grandmas1[i*40*3+j*3]
			+ 0.587*Grandmas1[i*40*3+j*3+1]
			+ 0.299*Grandmas1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Grandmas1[i*40*3+j*3]=Ih;
			Grandmas1[i*40*3+j*3+1]=Ih;
			Grandmas1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Mother1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				 
	face_cascade.detectMultiScale( frame_gray9, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Mother1.imageData[a*Mother1.widthStep+j*3];
			Buffer1[b+1] = Mother1.imageData[a*Mother1.widthStep+j*3+1];
			Buffer1[b+2] = Mother1.imageData[a*Mother1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Mothers1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Mothers1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Mothers1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Mothers1[i*40*3+j*3]
			+ 0.587*Mothers1[i*40*3+j*3+1]
			+ 0.299*Mothers1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Mothers1[i*40*3+j*3]
			+ 0.587*Mothers1[i*40*3+j*3+1]
			+ 0.299*Mothers1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Mothers1[i*40*3+j*3]=Ih;
			Mothers1[i*40*3+j*3+1]=Ih;
			Mothers1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Father1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////
	face_cascade.detectMultiScale( frame_gray10, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40,40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Father1.imageData[a*Father1.widthStep+j*3];
			Buffer1[b+1] = Father1.imageData[a*Father1.widthStep+j*3+1];
			Buffer1[b+2] = Father1.imageData[a*Father1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Fathers1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Fathers1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Fathers1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Fathers1[i*40*3+j*3]
			+ 0.587*Fathers1[i*40*3+j*3+1]
			+ 0.299*Fathers1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Fathers1[i*40*3+j*3]
			+ 0.587*Fathers1[i*40*3+j*3+1]
			+ 0.299*Fathers1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Fathers1[i*40*3+j*3]=Ih;
			Fathers1[i*40*3+j*3+1]=Ih;
			Fathers1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Girl1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				
	face_cascade.detectMultiScale( frame_gray11, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Girl1.imageData[a*Girl1.widthStep+j*3];
			Buffer1[b+1] = Girl1.imageData[a*Girl1.widthStep+j*3+1];
			Buffer1[b+2] = Girl1.imageData[a*Girl1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Girls1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Girls1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Girls1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Girls1[i*40*3+j*3]
			+ 0.587*Girls1[i*40*3+j*3+1]
			+ 0.299*Girls1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Girls1[i*40*3+j*3]
			+ 0.587*Girls1[i*40*3+j*3+1]
			+ 0.299*Girls1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Girls1[i*40*3+j*3]=Ih;
			Girls1[i*40*3+j*3+1]=Ih;
			Girls1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////Boy1Œv¹³Â^šú¥¿³W€Æ////////////////////////////////////////////////////////////////				 
	face_cascade.detectMultiScale( frame_gray12, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size2i(40, 40) );
	b=0;
	w=faces[0].width;
	h=faces[0].height;
	//Â^šú€HÁy
	for(int a=faces[0].y;a<faces[0].y + faces[0].height;a++){
		for(int j=faces[0].x;j<faces[0].x + faces[0].width;j++){
			Buffer1[b] = Boy1.imageData[a*Boy1.widthStep+j*3];
			Buffer1[b+1] = Boy1.imageData[a*Boy1.widthStep+j*3+1];
			Buffer1[b+2] = Boy1.imageData[a*Boy1.widthStep+j*3+2];
			b=b+3;
		}
	}
	//±N€HÁyÁY€pŠÜ40*40
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Boys1[i*40*3+j*3]=Buffer1[(i*h/40)*w*3+(j*w/40)*3];
			Boys1[i*40*3+j*3+1]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+1];
			Boys1[i*40*3+j*3+2]=Buffer1[(i*h/40)*w*3+(j*w/40)*3+2];
		}
	}
	Igray=0;Iall=0,Iavg=0,I=0,Ih=0;
	//°ªŠÇ¶¥Á`©M
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Boys1[i*40*3+j*3]
			+ 0.587*Boys1[i*40*3+j*3+1]
			+ 0.299*Boys1[i*40*3+j*3+2];
			Iall = Iall+Igray;
		}
	}
	//°ªŠÇ¶¥¥­§¡
	Iavg = Iall/(40*40);
	I=128-Iavg;
	//«G«×²Î€@(128-Iavg)+Igary
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			Igray = 0.114*Boys1[i*40*3+j*3]
			+ 0.587*Boys1[i*40*3+j*3+1]
			+ 0.299*Boys1[i*40*3+j*3+2];
			Ih=I+Igray;
			if(Ih>255)Ih=255;if(Ih<0)Ih=0;
			Boys1[i*40*3+j*3]=Ih;
			Boys1[i*40*3+j*3+1]=Ih;
			Boys1[i*40*3+j*3+2]=Ih;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//¥­§¡Áy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			favg = (Grandfas[i*40*3+j*3]+Grandmas[i*40*3+j*3]+Mothers[i*40*3+j*3]+Fathers[i*40*3+j*3]+Girls[i*40*3+j*3]+Boys[i*40*3+j*3]
			+Grandfas1[i*40*3+j*3]+Grandmas1[i*40*3+j*3]+Mothers1[i*40*3+j*3]+Fathers1[i*40*3+j*3]+Girls1[i*40*3+j*3]+Boys1[i*40*3+j*3])/12;
			VisionStatus::Favg[i*40*3+j*3]=favg;
			VisionStatus::Favg[i*40*3+j*3+1]=favg;
			VisionStatus::Favg[i*40*3+j*3+2]=favg;
		}
	}
	//Grandfa®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Grandfas[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			GrandfaD[i*40*3+j*3]=fd;
			GrandfaD[i*40*3+j*3+1]=fd;
			GrandfaD[i*40*3+j*3+2]=fd;
		}
	}
	//Grandma®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Grandmas[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			GrandmaD[i*40*3+j*3]=fd;
			GrandmaD[i*40*3+j*3+1]=fd;
			GrandmaD[i*40*3+j*3+2]=fd;
		}
	}
	//Mother®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Mothers[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			MotherD[i*40*3+j*3]=fd;
			MotherD[i*40*3+j*3+1]=fd;
			MotherD[i*40*3+j*3+2]=fd;
		}
	}
	//Father®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Fathers[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			FatherD[i*40*3+j*3]=fd;
			FatherD[i*40*3+j*3+1]=fd;
			FatherD[i*40*3+j*3+2]=fd;
		}
	}
	//Girl®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Girls[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			GirlD[i*40*3+j*3]=fd;
			GirlD[i*40*3+j*3+1]=fd;
			GirlD[i*40*3+j*3+2]=fd;
		}
	}
	//Boy®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Boys[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			BoyD[i*40*3+j*3]=fd;
			BoyD[i*40*3+j*3+1]=fd;
			BoyD[i*40*3+j*3+2]=fd;
		}
	}
	//Grandfa1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Grandfas1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Grandfa1D[i*40*3+j*3]=fd;
			Grandfa1D[i*40*3+j*3+1]=fd;
			Grandfa1D[i*40*3+j*3+2]=fd;
		}
	}
	//Grandma1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Grandmas1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Grandma1D[i*40*3+j*3]=fd;
			Grandma1D[i*40*3+j*3+1]=fd;
			Grandma1D[i*40*3+j*3+2]=fd;
		}
	}
	//Mother1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Mothers1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Mother1D[i*40*3+j*3]=fd;
			Mother1D[i*40*3+j*3+1]=fd;
			Mother1D[i*40*3+j*3+2]=fd;
		}
	}
	//Father1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Fathers1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Father1D[i*40*3+j*3]=fd;
			Father1D[i*40*3+j*3+1]=fd;
			Father1D[i*40*3+j*3+2]=fd;
		}
	}
	//Girl1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Girls1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Girl1D[i*40*3+j*3]=fd;
			Girl1D[i*40*3+j*3+1]=fd;
			Girl1D[i*40*3+j*3+2]=fd;
		}
	}
	//Boy1®t§OÁy
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			fd=Boys1[i*40*3+j*3]-VisionStatus::Favg[i*40*3+j*3];
			if(fd<0)fd=0;
			Boy1D[i*40*3+j*3]=fd;
			Boy1D[i*40*3+j*3+1]=fd;
			Boy1D[i*40*3+j*3+2]=fd;
		}
	}
	//±N5±i®t§OÁy³£©ñ¶iDallžÌ
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
	//±NdallÅÜŠš1600*5ªº«¬ºA
	Vector1=cvCreateMat(40*40,12,CV_32FC1);
	cvSetData(Vector1,Dall,Vector1->step);
	//¥­§¡­È«Å§i
	AvgVector=cvCreateMat(1,12,CV_32FC1);
	//¯SŒx­È«Å§i
	EigenValue_Row=cvCreateMat(12,1,CV_32FC1);
	//¯SŒxŠV¶q«Å§i
	EigenVector=cvCreateMat(12,12,CV_32FC1);
	//pca¹Bºâ
	cvCalcPCA(Vector1,AvgVector,EigenValue_Row,EigenVector,CV_PCA_DATA_AS_ROW);
	//§ä³Ì€j¯SŒx­È
	cp0=cvGet2D(EigenValue_Row,0,0).val[0];
	ui=0;
	for(int i=1;i<12;i++){
		cp1=cvGet2D(EigenValue_Row,i,0).val[0];
		if(cp1>cp0){
			cp0=cp1;
			ui=i;
		}
	}
	//¯SŒxÁy¹Bºâ
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
			VisionStatus::FeatureFace[i*40*3+(j*3)]=ff;
			VisionStatus::FeatureFace[i*40*3+(j*3)+1]=ff;
			VisionStatus::FeatureFace[i*40*3+(j*3)+2]=ff;
		}
	}
	//±N®t§OÁyÂàŠšfloat«¬ºA
	for(int i=0;i<40*40;i++){
		FeatureFaceW[i]=VisionStatus::FeatureFace[i*3];
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
	//±N¯SŒxÁyÂàŠš1*1600
	FFW=cvCreateMat(1,40*40,CV_32FC1);
	cvSetData(FFW,FeatureFaceW,FFW->step);
	//±N®t§OÁyÂàŠš1600*1
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
	//­pºâÅvŠV
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
	VisionStatus::cvGet2D_1 = cvGet2D(SumW1,0,0).val[0];
	VisionStatus::cvGet2D_2 = cvGet2D(SumW2,0,0).val[0];
	VisionStatus::cvGet2D_3 = cvGet2D(SumW3,0,0).val[0];
	VisionStatus::cvGet2D_4 = cvGet2D(SumW4,0,0).val[0];
	VisionStatus::cvGet2D_5 = cvGet2D(SumW5,0,0).val[0];
	VisionStatus::cvGet2D_6 = cvGet2D(SumW6,0,0).val[0];
	VisionStatus::cvGet2D_7 = cvGet2D(SumW7,0,0).val[0];
	VisionStatus::cvGet2D_8 = cvGet2D(SumW8,0,0).val[0];
	VisionStatus::cvGet2D_9 = cvGet2D(SumW9,0,0).val[0];
	VisionStatus::cvGet2D_10 = cvGet2D(SumW10,0,0).val[0];
	VisionStatus::cvGet2D_11 = cvGet2D(SumW11,0,0).val[0];
	VisionStatus::cvGet2D_12 = cvGet2D(SumW12,0,0).val[0];
}


