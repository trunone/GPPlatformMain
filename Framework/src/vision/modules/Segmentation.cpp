#include "Segmentation.h"
#include "VisionStatus.h"
#include "SegmentFunction.h"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace Robot;

Segmentation* Segmentation::m_UniqueInstance = new Segmentation();

Segmentation::Segmentation() {
}
Segmentation::~Segmentation() {
}
void Segmentation::Initialize() {
}

int Segmentation::Xcenter=0,Segmentation::Ycenter=0;

void Segmentation::Segment(unsigned char * TMPWebcamBoolBuffer)
{
	int temp=0,x1_temp=0,x2_temp=0,y1_temp=0,y2_temp=0;
	
	for(int i=1; i<VisionStatus::ImageWidth-1; i++){
		for(int j=1; j<VisionStatus::ImageHeight-1; j++){
			if(TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]==1){  //找到第一個被mark的點
				SegmentFunction::GetInstance()->SegmentationInit(i,j);
				TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]=0;  //清除標記
				int s;
				s=0;
				while (s <= VisionStatus::PointCnt){   //判斷是否抓完
					int x = SegmentFunction::GetInstance()->LocationList[s].x;  //pop
					int y = SegmentFunction::GetInstance()->LocationList[s].y;
					
					if(x == 0 || y == 0 || x==639 || y==479){  //防止超值(避免掃入邊緣點)
						s++;
						continue;
					}
					
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]=0;  
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]==1){  
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]==1){ 
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					s++;
					
				}
				
				//cout<<s;
				if (temp<s)
				{
					x1_temp=VisionStatus::Xmin;
					x2_temp=VisionStatus::Xmax;
					y1_temp=VisionStatus::Ymin;
					y2_temp=VisionStatus::Ymax;
					//cout << x1_temp <<"\t" <<x2_temp<<"\t"<<y1_temp<<"\t"<<y2_temp<<endl;
				}
				//cout<< s << "\t"<< temp <<endl;
				//cout << SegmentFunction::GetInstance()->LocationList.size()<<endl;
				SegmentFunction::GetInstance()->LocationList.clear();
					
			}
		}
	}
	VisionStatus::Xmin=x1_temp;
	VisionStatus::Xmax=x2_temp;
	VisionStatus::Ymin=y1_temp;
	VisionStatus::Ymax=y2_temp;
	
	
	Xcenter=(VisionStatus::Xmax+VisionStatus::Xmin)/2;
	Ycenter=(VisionStatus::Ymax+VisionStatus::Ymin)/2;
	//DrawLine(WebcamBoolBuffer);
}
void Segmentation::DrawLine(unsigned char *WebcamBuffer){ 
	for(int j=VisionStatus::Ymin; j < VisionStatus::Ymax; j++){
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmin)+2] = 255; 
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmin)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmin)+0] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmax)+2] = 255;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmax)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + VisionStatus::Xmax)+0] = 0;
	}
	for(int i=VisionStatus::Xmin; i < VisionStatus::Xmax; i++){
		WebcamBuffer[3*(VisionStatus::Ymin * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(VisionStatus::Ymin * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(VisionStatus::Ymin * VisionStatus::ImageWidth + i)+0] = 0;
		WebcamBuffer[3*(VisionStatus::Ymax * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(VisionStatus::Ymax * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(VisionStatus::Ymax * VisionStatus::ImageWidth + i)+0] = 0;
	}
}
void Segmentation::Process(){	
	Segment(VisionStatus::Blue_Ball);
	VisionStatus::Blue_X=Xcenter; 
	VisionStatus::Blue_Y=Ycenter;
	
	Segment(VisionStatus::Red_Ball);
	VisionStatus::Red_X=Xcenter;
	VisionStatus::Red_Y=Ycenter;
	
	Segment(VisionStatus::Green_Ball);
	VisionStatus::Green_X=Xcenter;
	VisionStatus::Green_Y=Ycenter;
	
	//cout<<VisionStatus::Green_X<<endl;
	//cout<<VisionStatus::Green_Y<<endl;
	
	cv::imwrite("2.jpg",VisionStatus::VideoFrame);
}
