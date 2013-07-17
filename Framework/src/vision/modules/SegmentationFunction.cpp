#include "SegmentationFunction.h"


using namespace std;
using namespace Robot;

int SegmentationFunction::Xmax(0);
int SegmentationFunction::Xmin(0);
int SegmentationFunction::Ymax(0);
int SegmentationFunction::Ymin(0);
int SegmentationFunction::PointCnt(0);
int SegmentationFunction::Xcenter(0);
int SegmentationFunction::Ycenter(0);


void SegmentationFunction::SegmentationInit(int Xvalue, int Yvalue)  //初始push第一個為0
{
	SegmentLocation node;
	node.x = Xvalue;
	node.y = Yvalue;
	LocationList.push_back(node);   
	Xmax = Xvalue; 
	Xmin = Xvalue;
	Ymax = Yvalue;
	Ymin = Yvalue;
	PointCnt = 0;
}
void SegmentationFunction::SegmentationInsert(int Xvalue, int Yvalue)
{
	SegmentLocation node;
	node.x = Xvalue;
	node.y = Yvalue;
	LocationList.push_back(node);
	if(Xvalue > Xmax)	Xmax = Xvalue;  //XY最大值最小值
	if(Xvalue < Xmin)	Xmin = Xvalue;
	if(Yvalue > Ymax)	Ymax = Yvalue;
	if(Yvalue < Ymin)	Ymin = Yvalue;
	PointCnt++;
}


void SegmentationFunction::Segment(unsigned char * TMPWebcamBoolBuffer, unsigned char * WebcamBoolBuffer)//物件抓取
{
	int temp=0,x1_temp=0,x2_temp=0,y1_temp=0,y2_temp=0;
	for(int i=1; i<VisionStatus::ImageWidth-1; i++){
		for(int j=1; j<VisionStatus::ImageHeight-1; j++){
			if(TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]==1){  //找到第一個被mark的點
				
				SegmentationInit(i,j);
				TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]=0;  //清除標記
				int s=0;
				while (s <= PointCnt){   //判斷是否抓完
					int x = LocationList[s].x;  //pop
					int y = LocationList[s].y;
					if(x == 0 || y == 0){  //防止超值(避免掃入邊緣點)
						s++;
						continue;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]==1){ //判斷周圍左上
						SegmentationInsert(x-1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]=0;  //消除標記
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]==1){  //上
						SegmentationInsert(x,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]==1){ //右上
						SegmentationInsert(x+1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]==1){  //左
						SegmentationInsert(x-1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]==1){  //右
						SegmentationInsert(x+1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]==1){  //左下
						SegmentationInsert(x-1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]==1){  //下
						SegmentationInsert(x,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]==1){  //右下
						SegmentationInsert(x+1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					s++;
				}  //抓到一個物體了!!!!
				if (temp<s)
				{
					temp=s;
					x1_temp=Xmin;
					x2_temp=Xmax;
					y1_temp=Ymin;
					y2_temp=Ymax;
				}
				
				
			}
		}
	}
	Xmin=x1_temp;
	Xmax=x2_temp;
	Ymin=y1_temp;
	Ymax=y2_temp;
	Xcenter=(Xmax+Xmin)/2;
	Ycenter=(Ymax+Ymin)/2;
	DrawLine(WebcamBoolBuffer);
}
void SegmentationFunction::DrawLine(unsigned char *WebcamBuffer){  //畫框框
		
	for(int j=Ymin; j < Ymax; j++){
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+2] = 255;  //紅線
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmin)+0] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+2] = 255;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + Xmax)+0] = 0;
	}
	for(int i=Xmin; i < Xmax; i++){
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(Ymin * VisionStatus::ImageWidth + i)+0] = 0;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(Ymax * VisionStatus::ImageWidth + i)+0] = 0;
	}
}
void SegmentationFunction::Process(){
	
	Segment(VisionStatus::Blue_Ball, VisionStatus::frame.data);
	VisionStatus::Blue_X=Xcenter; 
	VisionStatus::Blue_Y=Ycenter;
	Segment(VisionStatus::Red_Ball, VisionStatus::frame.data);
	VisionStatus::Red_X=Xcenter;
	VisionStatus::Red_Y=Ycenter;
	Segment(VisionStatus::Green_Ball, VisionStatus::frame.data);
	VisionStatus::Green_X=Xcenter;
	VisionStatus::Green_Y=Ycenter;
	cv::imwrite("a.jpg",VisionStatus::frame);
	
	//cvSaveImage(VisionStatus::send_frame.data,VisionStatus::frame.data);
}
