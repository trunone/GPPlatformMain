#include "Doornumber_detect.h"
#include <iostream>
using namespace Robot;
using namespace std;



Doornumber_detect* Doornumber_detect::m_UniqueInstance = new Doornumber_detect();

Doornumber_detect::Doornumber_detect() {
}
Doornumber_detect::~Doornumber_detect() {
}
void Doornumber_detect::Initialize() {
}

void Doornumber_detect::Gray_binarize(unsigned char *source,unsigned char *graybuffer){
	
	for(int j=0;j<480;j++){
		for(int i=0;i<640;i++){
			float reg=(source[(j*640+i)*3+0]+
			    source[(j*640+i)*3+1]+
			    source[(j*640+i)*3+2])/3;
			if(reg>=128){
			    	graybuffer[(j*640+i)*3+0]=0;
				graybuffer[(j*640+i)*3+1]=0;
				graybuffer[(j*640+i)*3+2]=0;
				
			}else{
			    	graybuffer[(j*640+i)*3+0]=255;
				graybuffer[(j*640+i)*3+1]=255;
				graybuffer[(j*640+i)*3+2]=255;				
			}

		}
	}	
}
void Doornumber_detect::Segment(unsigned char * TMPWebcamBoolBuffer){		//物件抓取
		int temp=0,x1_temp=0,x2_temp=0,y1_temp=0,y2_temp=0;
		for(int i=1; i<VisionStatus::ImageWidth-1; i++){
			for(int j=1; j<VisionStatus::ImageHeight-1; j++){
				if(TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]==2){  
					
					SegmentFunction::GetInstance()->SegmentationInit(i,j);
					TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]=0;  //清除標記
					int s=0;
					while (s <= VisionStatus::PointCnt){   //判斷是否抓完
						int x = SegmentFunction::GetInstance()->LocationList[s].x,  //pop
							y = SegmentFunction::GetInstance()->LocationList[s].y;
						if(x == 0 || y == 0){  //防止超值(避免掃入邊緣點)
							s++;
							continue;
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]==2){ //判斷周圍左上
							SegmentFunction::GetInstance()->SegmentationInsert(x-1,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]=0;  //消除標記
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]==2){  //上
							SegmentFunction::GetInstance()->SegmentationInsert(x,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]=0;
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]==2){ //右上
							SegmentFunction::GetInstance()->SegmentationInsert(x+1,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]=0;
						}
						if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]==2){  //左
							SegmentFunction::GetInstance()->SegmentationInsert(x-1,y);
							TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]=0;
						}
						if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]==2){  //右
							SegmentFunction::GetInstance()->SegmentationInsert(x+1,y);
							TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]==2){  //左下
							SegmentFunction::GetInstance()->SegmentationInsert(x-1,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]==2){  //下
							SegmentFunction::GetInstance()->SegmentationInsert(x,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]==2){  //右下
							SegmentFunction::GetInstance()->SegmentationInsert(x+1,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]=0;
						}
						s++;
					}  
					if (temp<s)
					{
						temp=s;
						x1_temp=VisionStatus::Xmin;
						x2_temp=VisionStatus::Xmax;
						y1_temp=VisionStatus::Ymin;
						y2_temp=VisionStatus::Ymax;
					}
					
					
				}
			}
		}
		VisionStatus::Xmin=x1_temp;
		VisionStatus::Xmax=x2_temp;
		VisionStatus::Ymin=y1_temp;
		VisionStatus::Ymax=y2_temp;
		
	}
void Doornumber_detect::merge(unsigned char *graybuffer	,unsigned char *door,unsigned char *mix){
	for(int WidthCnt = 0; WidthCnt < 640; WidthCnt++){
			for(int HeightCnt = 0; HeightCnt < 480; HeightCnt++){
				float temp=(graybuffer[HeightCnt * 640 + WidthCnt])/255+door[HeightCnt * 640 +WidthCnt];
				mix[(HeightCnt *640 + WidthCnt)]=temp;
			}		
	}
}
void Doornumber_detect::background_check(unsigned char *mix,int *b,int *w){
	for(int WidthCnt = (VisionStatus::Xmin-5); WidthCnt <= (VisionStatus::Xmax+5) ; WidthCnt++){
		for(int HeightCnt = (VisionStatus::Ymin-5);HeightCnt<=(VisionStatus::Ymax+5) ; HeightCnt++){
			if(mix[(HeightCnt *640 + WidthCnt)]==1){
				w=w+1;
			}else {
				if(mix[(HeightCnt *640 + WidthCnt)]==0){
				b=b+1;
				}
			}
		}
	}
}


void Doornumber_detect::Process(){
	Mat hsv;
	cvtColor(VisionStatus::VideoFrame, hsv, CV_BGR2HSV);
	unsigned char graybuffer[3*640*480];
	unsigned char Red_door[640*480];
	unsigned char Blue_door[640*480];
	unsigned char Green_door[640*480];
	unsigned char mix[640*480];
	int b=0,w=0;
	for(int WidthCnt = 0; WidthCnt < 640; WidthCnt++){
			for(int HeightCnt = 0; HeightCnt < 480; HeightCnt++){
						
				float hValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+0]*0.1/18;
				float sValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+1]*0.1/25.5;
				float vValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+2]*0.1/25.5;
				
				
				if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Red(hValue, sValue, vValue)){
					Red_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Red_door[(HeightCnt * 640 + WidthCnt)] = 0;	
				}
				if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Green(hValue, sValue, vValue)){
					Green_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Green_door[HeightCnt * 640 + WidthCnt] = 0;
				}
				if(ColorCheck::GetInstance()->HSV_hsvCheckRange_Blue(hValue, sValue, vValue)){
					Blue_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Blue_door[HeightCnt * 640 + WidthCnt] = 0;
				}				
			}
		}
	Gray_binarize(VisionStatus::VideoFrame.data,graybuffer);
	merge(graybuffer,Red_door,mix);
	Segment(Red_door);
	
	if(VisionStatus::Xmax>0
		&&VisionStatus::Xmin>=0
		&&VisionStatus::Ymax>0
		&&VisionStatus::Ymin>=0){
		background_check(mix,&b,&w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::door_status = 1; //red
		}else {
			b=0;
			w=0;
		}
	}
	merge(graybuffer,Green_door,mix);
	Segment(Green_door);
	if(VisionStatus::Xmax>0
		&&VisionStatus::Xmin>=0
		&&VisionStatus::Ymax>0
		&&VisionStatus::Ymin>=0){
		background_check(mix,&b,&w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::door_status = 2; //green
		}else {
			b=0;
			w=0;
		}
	}
	merge(graybuffer,Blue_door,mix);
	Segment(Blue_door);
	if(VisionStatus::Xmax>0
		&&VisionStatus::Xmin>=0
		&&VisionStatus::Ymax>0
		&&VisionStatus::Ymin>=0){
		background_check(mix,&b,&w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::door_status = 3; //blue
		}else {
			b=0;
			w=0;
		}
	}
	

}