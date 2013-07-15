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
		static int ImageWidth, ImageHeight;

		static cv::Mat frame;
		

		//--- ModelUnit
		static ColorRange hsvGreenRange, hsvBlueRange, hsvRedRange;

		static int ColorDeep;

		static int xImageCenter, yImageCenter;
		static int GlobalScanMagnEnd;

		static float AngularToRadian,RadianToAngular;


		static float ComputeCenterAngle(float AngleStartPoint, float AngleEndPoint);
		
	

	};
}

#endif
