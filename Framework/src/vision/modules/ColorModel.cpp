#include "ColorModel.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "VisionStatus.h"

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


void ColorModel::Process(){ 
	unsigned char temp [VisionStatus::ImageWidth * VisionStatus::ImageHeight];
	
	cv::Mat hsv;
	cv::cvtColor(VisionStatus::VideoFrame, hsv, CV_BGR2HSV);
	
	for(int WidthCnt = 0; WidthCnt < VisionStatus::ImageWidth; WidthCnt++){
		for(int HeightCnt = 0; HeightCnt < VisionStatus::ImageHeight; HeightCnt++){
						
			float hValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+0]*0.1/18;
			float sValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+1]*0.1/25.5;
			float vValue = hsv.data[3*(HeightCnt * VisionStatus::ImageWidth + WidthCnt)+2]*0.1/25.5;
			//cout<<sValue<<endl;
			//------catch Blue
			if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Blue(hValue, sValue, vValue)){					 
				VisionStatus::Blue_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;	
				
			}else{
				VisionStatus::Blue_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;	
				
			}
			//------catch Green
			if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Green(hValue, sValue, vValue)){
				VisionStatus::Green_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;
			}else{
				VisionStatus::Green_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;
			}
			//------catch red
			if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Red(hValue, sValue, vValue)){
				VisionStatus::Red_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 1;
			}else{
				VisionStatus::Red_Ball[(HeightCnt * VisionStatus::ImageWidth + WidthCnt)] = 0;
			}
		}
	}

	
}

