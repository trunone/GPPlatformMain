#include "Segmentation.h"


using namespace std;
using namespace Robot;

Segmentation* Segmentation::m_UniqueInstance = new Segmentation();

Segmentation::Segmentation() {
}
Segmentation::~Segmentation() {
}
void Segmentation::Initialize() {
}

int Xcenter=0,Ycenter=0;

void Segmentation::Segment(unsigned char * TMPWebcamBoolBuffer, unsigned char * WebcamBoolBuffer)//物件抓取
{
	int temp=0,x1_temp=0,x2_temp=0,y1_temp=0,y2_temp=0;
	for(int i=1; i<VisionStatus::ImageWidth-1; i++){
		for(int j=1; j<VisionStatus::ImageHeight-1; j++){
			if(TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]==1){  //找到第一個被mark的點
				
				SegmentFunction::GetInstance()->SegmentationInit(i,j);
				TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]=0;  //清除標記
				int s=0;
				while (s <= SegmentFunction::GetInstance()->PointCnt){   //判斷是否抓完
					int x = SegmentFunction::GetInstance()->LocationList[s].x;  //pop
					int y = SegmentFunction::GetInstance()->LocationList[s].y;
					if(x == 0 || y == 0){  //防止超值(避免掃入邊緣點)
						s++;
						continue;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]==1){ //判斷周圍左上
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]=0;  //消除標記
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]==1){  //上
						SegmentFunction::GetInstance()->SegmentationInsert(x,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]==1){ //右上
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]==1){  //左
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]==1){  //右
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]==1){  //左下
						SegmentFunction::GetInstance()->SegmentationInsert(x-1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]==1){  //下
						SegmentFunction::GetInstance()->SegmentationInsert(x,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]==1){  //右下
						SegmentFunction::GetInstance()->SegmentationInsert(x+1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					s++;
				}  //抓到一個物體了!!!!
				if (temp<s)
				{
					temp=s;
					x1_temp=SegmentFunction::GetInstance()->Xmin;
					x2_temp=SegmentFunction::GetInstance()->Xmax;
					y1_temp=SegmentFunction::GetInstance()->Ymin;
					y2_temp=SegmentFunction::GetInstance()->Ymax;
				}
				
				
			}
		}
	}
	SegmentFunction::GetInstance()->Xmin=x1_temp;
	SegmentFunction::GetInstance()->Xmax=x2_temp;
	SegmentFunction::GetInstance()->Ymin=y1_temp;
	SegmentFunction::GetInstance()->Ymax=y2_temp;
	Xcenter=(SegmentFunction::GetInstance()->Xmax+SegmentFunction::GetInstance()->Xmin)/2;
	Ycenter=(SegmentFunction::GetInstance()->Ymax+SegmentFunction::GetInstance()->Ymin)/2;
	DrawLine(WebcamBoolBuffer);
}
void Segmentation::DrawLine(unsigned char *WebcamBuffer){  //畫框框
		
	for(int j=SegmentFunction::GetInstance()->Ymin; j < SegmentFunction::GetInstance()->Ymax; j++){
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+2] = 255;  //紅線
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+0] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+2] = 255;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+0] = 0;
	}
	for(int i=SegmentFunction::GetInstance()->Xmin; i < SegmentFunction::GetInstance()->Xmax; i++){
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+0] = 0;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+0] = 0;
	}
}
void Segmentation::Process(){
	
	Segment(VisionStatus::Blue_Ball, VisionStatus::frame.data);
	VisionStatus::Blue_X=Xcenter; 
	VisionStatus::Blue_Y=Ycenter;
	Segment(VisionStatus::Red_Ball, VisionStatus::frame.data);
	VisionStatus::Red_X=Xcenter;
	VisionStatus::Red_Y=Ycenter;
	Segment(VisionStatus::Green_Ball, VisionStatus::frame.data);
	VisionStatus::Green_X=Xcenter;
	VisionStatus::Green_Y=Ycenter;
	cv::imwrite("a.jpg",VisionStatus::VideoFrame);
	
	
}
