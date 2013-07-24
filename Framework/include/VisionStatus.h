/*
 *   VisionStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _VISION_STATUS_H_
#define _VISION_STATUS_H_
#include <vector>
#include <string>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define M_PI       3.14159265358979323846

using namespace std;
using namespace cv;
//---------------------------------------------------------------------------


namespace Robot
{
	class VisionStatus
	{

	private:

	public:
		struct ColorRange
		{
			float HueMax;
			float HueMin;
			float SaturationMax;
			float SaturationMin;
			float BrightnessMax;
			float BrightnessMin;
		};

		

		//---seg
		
		/*static int Xmax, Xmin, Ymax, Ymin;
		static int PointCnt;
		static void SegmentationInit(int Xvalue, int Yvalue);
		static void SegmentationInsert(int Xvalue, int Yvalue);
		static vector <SegmentLocation> LocationList;*/
	
		//--- Global
		static const int ImageWidth = 640, ImageHeight = 480;

		static cv::Mat frame;
		static cv::Mat send_frame;
		
		static unsigned char Blue_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight],
				     Green_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight],
				     Red_Ball[VisionStatus::ImageWidth * VisionStatus::ImageHeight];
		static int Red_X,Red_Y,Green_X,Green_Y,Blue_X,Blue_Y;

		//--- ModelUnit
		static ColorRange hsvGreenRange, hsvBlueRange, hsvRedRange;

		static int ColorDeep;

		static int xImageCenter, yImageCenter;
		static int GlobalScanMagnEnd;

		static float AngularToRadian,RadianToAngular;


		static float ComputeCenterAngle(float AngleStartPoint, float AngleEndPoint);
		
		//------ FaceDetection ----------------------------------------------------------
		static Mat VideoFrame;

		//------ FaceData ----------------------------------------------------------
		static unsigned char Favg[40*40*3];
		static float FeatureFaceW[40*40*3];
		static float cvGet2D_1;
		static float cvGet2D_2;
		static float cvGet2D_3;
		static float cvGet2D_4;
		static float cvGet2D_5;
		static float cvGet2D_6;
		static float cvGet2D_7;
		static float cvGet2D_8;
		static float cvGet2D_9;
		static float cvGet2D_10;
		static float cvGet2D_11;
		static float cvGet2D_12;	

	};
}

#endif
