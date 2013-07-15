#include "Colormodel.h"
#include"cv"
using namespace Robot;


bool Colormodel::HSV_hsvCheckRange_Blue(VisionStatus::ColorRange VisionStatus::hsvBlueRange, float hValue, float sValue, float vValue){  
	if(VisionStatus::hsvBlueRange.HueMax >= VisionStatus::hsvBlueRange.HueMin){   
		if(VisionStatus::hsvBlueRange.HueMax >= hValue && VisionStatus::hsvBlueRange.HueMin <= hValue
	   	&& VisionStatus::hsvBlueRange.SaturationMax >= sValue && VisionStatus::hsvBlueRange.SaturationMin <= sValue
	   	&& VisionStatus::hsvBlueRange.BrightnessMax >= vValue && VisionStatus::hsvBlueRange.BrightnessMin <= vValue)
		return true;
	}else{
		if( (VisionStatus::hsvBlueRange.HueMax >= hValue || VisionStatus::hsvBlueRange.HueMin <= hValue)
		&& VisionStatus::hsvBlueRange.SaturationMax >= sValue && VisionStatus::hsvBlueRange.SaturationMin <= sValue
		&& VisionStatus::hsvBlueRange.BrightnessMax >= vValue && VisionStatus::hsvBlueRange.BrightnessMin <= vValue)
		return true;
	}
	        return false;
}
bool Colormodel::HSV_hsvCheckRange_Red(VisionStatus::ColorRange VisionStatus::hsvRedRange, float hValue, float sValue, float vValue){  
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
bool Colormodel::HSV_hsvCheckRange_Green(VisionStatus::ColorRange VisionStatus::hsvGreenRange, float hValue, float sValue, float vValue){  
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

void Colormodel::ColorProcess(unsigned char * WebcamBuffer){ 
	//------get hsv value
	
	//------catch Blue
	if(Colormodel::HSV_hsvCheckRange_Blue(&colorrange, hValue, sValue, vValue)){
					 
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2] = 0;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1] = 255;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0] = 255;
			
	}
	SegmentationFunction::SegmentationFunction(WebcamBoolBuffer,/*origin picture*/);
	//------catch red
	if(Colormodel::HSV_hsvCheckRange_Red(&colorrange, hValue, sValue, vValue)){
						 
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2] = 255;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1] = 0;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0] = 0;
				
	}
	SegmentationFunction::SegmentationFunction(WebcamBoolBuffer,/*origin picture*/);
	//------catch Green
	if(Colormodel::HSV_hsvCheckRange_Green(&colorrange, hValue, sValue, vValue)){
					 
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2] = 0;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1] = 255;
		WebcamBuffer[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0] = 0;
	}
	SegmentationFunction::SegmentationFunction(WebcamBoolBuffer,/*origin picture*/);
	
	
}

