bool HSV_hsvCheckRange(ColorRange* hsvRange, float hValue, float sValue, float vValue){ //判斷顏色範圍 
	if(VisionStatus::hsvYellowRange.HueMax >= VisionStatus::hsvYellowRange.HueMin){   //小於最大值,大於最小值時回true回傳
		if(VisionStatus::hsvYellowRange.HueMax >= hValue && VisionStatus::hsvYellowRange.HueMin <= hValue
	   	&& VisionStatus::hsvYellowRange.SaturationMax >= sValue && VisionStatus::hsvYellowRange.SaturationMin <= sValue
	   	&& VisionStatus::hsvYellowRange.BrightnessMax >= vValue && VisionStatus::hsvYellowRange.BrightnessMin <= vValue)
		return true;
	}else{
		if( (VisionStatus::hsvYellowRange.HueMax >= hValue || VisionStatus::hsvYellowRange.HueMin <= hValue)
		&& VisionStatus::hsvYellowRange.SaturationMax >= sValue && VisionStatus::hsvYellowRange.SaturationMin <= sValue
		&& VisionStatus::hsvYellowRange.BrightnessMax >= vValue && VisionStatus::hsvYellowRange.BrightnessMin <= vValue)
		return true;
	}
	        return false;
}
void ColorProcess(unsigned char * WebcamBuffer){ //色模
	
	for(int WidthCnt = 0; WidthCnt < DefWebcanWidth; WidthCnt++){
		for(int HeightCnt = 0; HeightCnt < DefWebcanHeight; HeightCnt++){
			int R = WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+2];
			int G = WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+1];
			int B = WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+0];

			float rValue = R/255.0; //最大範圍為1,0來判斷
			float gValue = G/255.0;
			float bValue = B/255.0;

			float hValue = this->ColorFunctionUnit->HofRGBtoHSV(rValue, gValue, bValue);
			float sValue = this->ColorFunctionUnit->SofRGBtoHSV(rValue, gValue, bValue);
			float vValue = this->ColorFunctionUnit->VofRGBtoHSV(rValue, gValue, bValue);

			if(this->HSV_hsvCheckRange(&colorrange, hValue, sValue, vValue)){
						 
				WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+2] = 255;
				WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+1] = 0;
				WebcamBuffer[3*(HeightCnt * DefWebcanWidth + WidthCnt)+0] = 0;
			}
								 
		}
	}
}

