#include "ColorModel.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "VisionStatus.h"
#include "SegmentationFunction.h"

using namespace Robot;
using namespace std;

ColorModel* ColorModel::m_UniqueInstance = new ColorModel();

ColorModel::ColorModel() {
}
ColorModel::~ColorModel() {
}
void ColorModel::Initialize() {
}
//-------------------------------xml
int ColorModel::LoadXMLSettings(TiXmlElement* element){
	TiXmlElement* modelchild;
	modelchild=element->FirstChildElement("Blue");
	if(modelchild != NULL){
		modelchild->Attribute("HueMax", &VisionStatus::hsvBlueRange.HueMax);
		modelchild->Attribute("HueMin", &VisionStatus::hsvBlueRange.HueMin);
		modelchild->Attribute("SaturationMax", &VisionStatus::hsvBlueRange.SaturationMax);
		modelchild->Attribute("SaturationMin", &VisionStatus::hsvBlueRange.SaturationMin);
		modelchild->Attribute("BrightnessMax", &VisionStatus::hsvBlueRange.BrightnessMax);
		modelchild->Attribute("BrightnessMin", &VisionStatus::hsvBlueRange.BrightnessMin);
	}
	modelchild=element->FirstChildElement("Green");
	if(modelchild != NULL){
		modelchild->Attribute("HueMax", &VisionStatus::hsvGreenRange.HueMax);
		modelchild->Attribute("HueMin", &VisionStatus::hsvGreenRange.HueMin);
		modelchild->Attribute("SaturationMax", &VisionStatus::hsvGreenRange.SaturationMax);
		modelchild->Attribute("SaturationMin", &VisionStatus::hsvGreenRange.SaturationMin);
		modelchild->Attribute("BrightnessMax", &VisionStatus::hsvGreenRange.BrightnessMax);
		modelchild->Attribute("BrightnessMin", &VisionStatus::hsvGreenRange.BrightnessMin);
	}
	modelchild=element->FirstChildElement("Red");
	if(modelchild != NULL){
		modelchild->Attribute("HueMax", &VisionStatus::hsvRedRange.HueMax);
		modelchild->Attribute("HueMin", &VisionStatus::hsvRedRange.HueMin);
		modelchild->Attribute("SaturationMax", &VisionStatus::hsvRedRange.SaturationMax);
		modelchild->Attribute("SaturationMin", &VisionStatus::hsvRedRange.SaturationMin);
		modelchild->Attribute("BrightnessMax", &VisionStatus::hsvRedRange.BrightnessMax);
		modelchild->Attribute("BrightnessMin", &VisionStatus::hsvRedRange.BrightnessMin);
	}
	return 0;
}
//----------------------------------
bool ColorModel::HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue){  
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
bool ColorModel::HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue){  
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
bool ColorModel::HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue){  
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
void ColorModel::Erosion(unsigned char *source, unsigned char *target){
	for(int j=1;j<(VisionStatus::ImageHeight-1);j++){
		for(int i=1;i<(VisionStatus::ImageWidth-1);i++){
				 target[j*640+i]=source[(j-1)*640+(i-1)]&source[(j-1)*640+(i)]&source[(j-1)*640+(i+1)]&
						 source[(j)*640+(i-1)]&source[(j-1)*640+(i+1)]&source[(j+1)*640+(i-1)]&
						 source[(j+1)*640+(i)]&source[(j+1)*640+(i+1)];
	}
		}
}
void ColorModel::Dilation(unsigned char *source, unsigned char *target){
	for(int j=1;j<(VisionStatus::ImageHeight-1);j++){
		for(int i=1;i<(VisionStatus::ImageWidth-1);i++){
			target[j*640+i]=source[(j-1)*640+(i-1)]|source[(j-1)*640+(i)]|source[(j-1)*640+(i+1)]|
					source[(j)*640+(i-1)]|source[(j-1)*640+(i+1)]|source[(j+1)*640+(i-1)]|
					source[(j+1)*640+(i)]|source[(j+1)*640+(i+1)];
		}
	}
}
void ColorModel::Process(){ 
	unsigned char temp [VisionStatus::ImageWidth * VisionStatus::ImageHeight];
	
	cv::Mat hsv;
	cv::cvtColor(VisionStatus::frame, hsv, CV_BGR2HSV);
	for(int WidthCnt = 0; WidthCnt < VisionStatus::ImageWidth; WidthCnt++){
		for(int HeightCnt = 0; HeightCnt < VisionStatus::ImageHeight; HeightCnt++){
						
			float hValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0];
			float sValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1];
			float vValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2];
			//------catch Blue
			if(HSV_hsvCheckRange_Blue(hValue, sValue, vValue)){					 
				VisionStatus::Blue_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;
			}else{
				VisionStatus::Blue_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;			
			}
		}
	}
	Erosion(VisionStatus::Blue_Ball,temp);
	Dilation(temp,VisionStatus::Blue_Ball);
	//SegmentationFunction::SegmentationFunction(TMPWebcamBoolBuffer,VisionStatus::frame.data);
	
	for(int WidthCnt = 0; WidthCnt < VisionStatus::ImageWidth; WidthCnt++){
		for(int HeightCnt = 0; HeightCnt < VisionStatus::ImageHeight; HeightCnt++){
						
			float hValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0];
			float sValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1];
			float vValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2];
			//------catch red
			if(HSV_hsvCheckRange_Red(hValue, sValue, vValue)){
				VisionStatus::Red_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;
			}else{
				VisionStatus::Red_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;
			}
		}
	}
	Erosion(VisionStatus::Red_Ball,temp);
	Dilation(temp,VisionStatus::Red_Ball);
	//SegmentationFunction::SegmentationFunction(TMPWebcamBoolBuffer,VisionStatus::frame.data);

	for(int WidthCnt = 0; WidthCnt < VisionStatus::ImageWidth; WidthCnt++){
		for(int HeightCnt = 0; HeightCnt < VisionStatus::ImageHeight; HeightCnt++){
						
			float hValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0];
			float sValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1];
			float vValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2];
			//------catch Green
			if(HSV_hsvCheckRange_Green(hValue, sValue, vValue)){
				VisionStatus::Green_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;
			}else{
				VisionStatus::Green_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;
			}
		}
	}
	Erosion(VisionStatus::Green_Ball,temp);
	Dilation(temp,VisionStatus::Green_Ball);
	//SegmentationFunction::SegmentationFunction(TMPWebcamBoolBuffer,VisionStatus::frame.data);
}

