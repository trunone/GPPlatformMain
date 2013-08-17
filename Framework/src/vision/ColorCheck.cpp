#include "ColorCheck.h"
#include "VisionStatus.h"
#include <iostream>
using namespace Robot;
using namespace std;

ColorCheck* ColorCheck::m_UniqueInstance = new ColorCheck();

ColorCheck::ColorCheck() {
}
ColorCheck::~ColorCheck() {
}


bool ColorCheck::HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue){  
	if(VisionStatus::hsvBlueRange.HueMax >= VisionStatus::hsvBlueRange.HueMin){   
		if(VisionStatus::hsvBlueRange.HueMax >= hValue && VisionStatus::hsvBlueRange.HueMin <= hValue
	   	&& VisionStatus::hsvBlueRange.SaturationMax >= sValue && VisionStatus::hsvBlueRange.SaturationMin <= sValue
	   	&& VisionStatus::hsvBlueRange.BrightnessMax >= vValue && VisionStatus::hsvBlueRange.BrightnessMin <= vValue)		
		return true;
	}else{
		if( (VisionStatus::hsvBlueRange.HueMax >= hValue || VisionStatus::hsvBlueRange.HueMin <= hValue)
		&& VisionStatus::hsvBlueRange.SaturationMax >= sValue && VisionStatus::hsvBlueRange.SaturationMin <= sValue
		&& VisionStatus::hsvBlueRange.BrightnessMax >= vValue && VisionStatus::hsvBlueRange.BrightnessMin <= vValue){
		
		return true;
		}
	}
		
	        return false;
}
bool ColorCheck::HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue){  
	if(VisionStatus::hsvRedRange.HueMax >= VisionStatus::hsvRedRange.HueMin){   
		if(VisionStatus::hsvRedRange.HueMax >= hValue && VisionStatus::hsvRedRange.HueMin <= hValue
	   	&& VisionStatus::hsvRedRange.SaturationMax >= sValue && VisionStatus::hsvRedRange.SaturationMin <= sValue
	   	&& VisionStatus::hsvRedRange.BrightnessMax >= vValue && VisionStatus::hsvRedRange.BrightnessMin <= vValue)
			return true;
	}else{
		if( (VisionStatus::hsvRedRange.HueMax >= hValue || VisionStatus::hsvRedRange.HueMin <= hValue)
		&& VisionStatus::hsvRedRange.SaturationMax >= sValue && VisionStatus::hsvRedRange.SaturationMin <= sValue
		&& VisionStatus::hsvRedRange.BrightnessMax >= vValue && VisionStatus::hsvRedRange.BrightnessMin <= vValue)
			return true;
	}
	        return false;
}
bool ColorCheck::HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue){  
	if(VisionStatus::hsvGreenRange.HueMax >= VisionStatus::hsvGreenRange.HueMin){   
		if(VisionStatus::hsvGreenRange.HueMax >= hValue && VisionStatus::hsvGreenRange.HueMin <= hValue
	   	&& VisionStatus::hsvGreenRange.SaturationMax >= sValue && VisionStatus::hsvGreenRange.SaturationMin <= sValue
	   	&& VisionStatus::hsvGreenRange.BrightnessMax >= vValue && VisionStatus::hsvGreenRange.BrightnessMin <= vValue)
			return true;
	}else{
		if( (VisionStatus::hsvGreenRange.HueMax >= hValue || VisionStatus::hsvGreenRange.HueMin <= hValue)
		&& VisionStatus::hsvGreenRange.SaturationMax >= sValue && VisionStatus::hsvGreenRange.SaturationMin <= sValue
		&& VisionStatus::hsvGreenRange.BrightnessMax >= vValue && VisionStatus::hsvGreenRange.BrightnessMin <= vValue)
			return true;
	}
	        return false;
}
