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
#include <stdio.h>
#include <stdlib.h>



using namespace Robot;
/*
VisionStatus::tsColor colorBGR (unsigned char B,unsigned char G,unsigned char R)

//unsigned char *VisionStatus::pImage = new unsigned char[3 * VisionStatus::ImageWidth * VisionStatus::ImageHeight];
//-----cvimage
cv::Mat VisionStatus::frame;
cv::Mat VisionStatus::send_frame;

//-----ObjCut
unsigned char VisionStatus::Blue_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Green_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
unsigned char VisionStatus::Red_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];

//-----ball posistion
int VisionStatus::Red_X(0);
int VisionStatus::Red_Y(0);
int VisionStatus::Green_X(0);
int VisionStatus::Green_Y(0);
int VisionStatus::Blue_X(0); 
int VisionStatus::Blue_Y(0); 

VisionStatus::ColorRange VisionStatus::hsvBlueRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvRedRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

VisionStatus::ColorRange VisionStatus::hsvGreenRange = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0};


int VisionStatus::ColorDeep(256);
int VisionStatus::xImageCenter(320);
int VisionStatus::yImageCenter(240);
int VisionStatus::GlobalScanMagnEnd(237);

float VisionStatus::AngularToRadian(M_PI / 180.0);
float VisionStatus::RadianToAngular(180.0 / M_PI);
float VisionStatus::ComputeCenterAngle(float AngleStartPoint, float AngleEndPoint)
{
	float tmpAngle;

	if(AngleEndPoint < AngleStartPoint)
	{
		if(AngleStartPoint < 0.0)
			AngleStartPoint += (2.0 * M_PI);
		if(AngleEndPoint < 0.0)
			AngleEndPoint += (2.0 * M_PI);
	}
	tmpAngle = (AngleEndPoint - AngleStartPoint) / 2.0 + AngleStartPoint;

	//else
	//{
	//        tmpAngle = (AngleEndPoint + AngleStartPoint) / 2.0;
	//}

	if(tmpAngle > M_PI)
		tmpAngle -= (2.0 * M_PI);

	return tmpAngle;
}
/*

{
    VisionStatus::tsColor color;
    color.ColorEle[BLUE]     = B ;
    color.ColorEle[GREEN]    = G ;
    color.ColorEle[RED]      = R ;
    return color ;
}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
//-----------------------tsBmpPtr-------------------------------------
VisionStatus::tsColor tsBmpPtr::GetColor(int x,int y)
{
    if(x<Width && y<Height && x>=0 && y>=0){
        VisionStatus::tsColor* ColorPtr;
        ColorPtr = (VisionStatus::tsColor*)ImgData;
        return  ColorPtr[x+y*Width];
    }
    else return cBLACK;
}

unsigned char* tsBmpPtr::GetPixelPtr(int x,int y)
{
    if(x<Width && y<Height && x>=0 && y>=0){
        return  &ImgData[Dimension*(x+y*Width)];
    }
    else return NULL;
}

void tsBmpPtr::SetColor(int x,int y ,tsColor cBGR)
{
    if(x<Width && y<Height && x>=0 && y>=0){
        tsColor* ColorPtr;
        ColorPtr = (tsColor*)ImgData;
        ColorPtr[x+y*Width]= cBGR;
    }
}

void tsBmpPtr::SetColor(int x,int y ,unsigned char* Color)
{
    if(x<Width && y<Height && x>=0 && y>=0){
        for( int i=0 ; i<Dimension ; i++ )
            ImgData[Dimension*(x+y*Width)+i] = Color[i];
    }
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//---------------------tsImgProSet------------------------------------
float tsImgDisModel::Pixel2Distance(int pixel)
{
    if (enable){
        if (ModelNum == NaN) return NaN;
        else{
            if      (pixel > this->DisPixel_Model[ModelNum-2].Pixel) return NaN ;  //this last data is end of data must be zero 
            else if (pixel < this->DisPixel_Model[    0    ].Pixel) return NaN ;
            else{
                int offset = pixel-this->DisPixel_Model[0].Pixel;
                return this->DisPixel_Model[offset].Distance;
            }
        }
    }
    else  return NaN ;
}
*/

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//TImg(void);
//~TImg(void);
//unsigned char ColorSegmentation(unsigned char yValue, unsigned char uValue, unsigned char vValue);
//double ComputeCenterAngle(double AngleStartPoint, double AngleEndPoint);
//void LoadModelFromFile(AnsiString FileString);
//void SaveColorRangeFormFile(AnsiString FileString);
//void LoadColorRangeFormFile(AnsiString FileString);

//int ImageWidth, ImageHeight, ImageByte;		//影像的寬、高、大小
//int ImageLevel;
//double AngularToRadian, RadianToAngular;
//ColorRange *hsvBlueRange, *hsvBlackRange, *hsvGreenRange, *hsvOrangeRange, *hsvYellowRange, *hsvRedRange;
//unsigned char *pImage;
//unsigned char *pWebCamImage;
//unsigned char *pFindEdgeImage;
VisionStatus::tsObjectiveInfo BallInfo;
VisionStatus::tsBmpPtr Image;
VisionStatus::tsImgProSet ProcessSetting;
VisionStatus::tsImgDisModel DisModel;
VisionStatus::tsYUVData *RGBtoYUV_Table;

unsigned char PixelGrayScale(VisionStatus::tsColor Pixel);
bool PixelBinarization(VisionStatus::tsColor Pixel, unsigned char Threshold);
void ImgGrayScale(VisionStatus::tsBmpPtr Image);
void ImgBinarization(VisionStatus::tsBmpPtr Image, unsigned char Threshold);


//------ FaceData ----------------------------------------------------------
unsigned char VisionStatus::Favg[40*40*3];
float VisionStatus::FeatureFaceW[40*40*3];
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



