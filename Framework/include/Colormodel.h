#ifndef _Colormodel_H_
#define _Colormodel_H_
#include "VisionStatus.h"
#include "SegmentationFunction.h"

using namespace std;
//----------------------------

namespace Robot
{
	class Colormodel
	{

	private:
		bool HSV_hsvCheckRange_Yellow(VisionStatus::ColorRange VisionStatus::hsvYellowRange, float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Red(VisionStatus::ColorRange VisionStatus::hsvRedRange, float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Green(VisionStatus::ColorRange VisionStatus::hsvGreenRange, float hValue, float sValue, float vValue);

		void ColorProcess(unsigned char * WebcamBuffer);
	};
}
#endif
