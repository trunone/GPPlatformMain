#include "SegmentationFunction.h"
#include "VisionStatus.h"

using namespace std;
using namespace Robot;

int SegmentationFunction::Xmax(0);
int SegmentationFunction::Xmin(0);
int SegmentationFunction::Ymax(0);
int SegmentationFunction::Ymin(0);
int SegmentationFunction::PointCnt(0);

void SegmentationFunction::SegmentationInit(int Xvalue, int Yvalue)  //初始push第一個為0
{
	SegmentLocation node;
	node.x = Xvalue;
	node.y = Yvalue;
	LocationList.push_back(node);   
	SegmentationFunction::Xmax = Xvalue; 
	SegmentationFunction::Xmin = Xvalue;
	SegmentationFunction::Ymax = Yvalue;
	SegmentationFunction::Ymin = Yvalue;
	SegmentationFunction::PointCnt = 0;
}
void SegmentationFunction::SegmentationInsert(int Xvalue, int Yvalue)
{
	SegmentLocation node;
	node.x = Xvalue;
	node.y = Yvalue;
	LocationList.push_back(node);
	if(Xvalue > SegmentationFunction::Xmax)	SegmentationFunction::Xmax = Xvalue;  //XY最大值最小值
	if(Xvalue < SegmentationFunction::Xmin)	SegmentationFunction::Xmin = Xvalue;
	if(Yvalue > SegmentationFunction::Ymax)	SegmentationFunction::Ymax = Yvalue;
	if(Yvalue < SegmentationFunction::Ymin)	SegmentationFunction::Ymin = Yvalue;
	SegmentationFunction::PointCnt++;
}


SegmentationFunction::SegmentationFunction(unsigned char * TMPWebcamBoolBuffer, unsigned char * WebcamBoolBuffer)//物件抓取
{
	int temp=0,x1_temp=0,x2_temp=0,y1_temp=0,y2_temp=0;
	for(int i=1; i<VisionStatus::ImageWidth-1; i++){
		for(int j=1; j<VisionStatus::ImageHeight-1; j++){
			if(TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]==1){  //找到第一個被mark的點
				
				SegmentationFunction::SegmentationInit(i,j);
				TMPWebcamBoolBuffer[j * VisionStatus::ImageWidth + i]=0;  //清除標記
				int s=0;
				while (s <= SegmentationFunction::PointCnt){   //判斷是否抓完
					int x = SegmentationFunction::LocationList[s].x;  //pop
					int y = SegmentationFunction::LocationList[s].y;
					if(x == 0 || y == 0){  //防止超值(避免掃入邊緣點)
						s++;
						continue;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]==1){ //判斷周圍左上
						SegmentationFunction::SegmentationInsert(x-1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x-1)]=0;  //消除標記
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]==1){  //上
						SegmentationFunction::SegmentationInsert(x,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]==1){ //右上
						SegmentationFunction::SegmentationInsert(x+1,y-1);
						TMPWebcamBoolBuffer[(y-1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]==1){  //左
						SegmentationFunction::SegmentationInsert(x-1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]==1){  //右
						SegmentationFunction::SegmentationInsert(x+1,y);
						TMPWebcamBoolBuffer[y * VisionStatus::ImageWidth + (x+1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]==1){  //左下
						SegmentationFunction::SegmentationInsert(x-1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x-1)]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]==1){  //下
						SegmentationFunction::SegmentationInsert(x,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + x]=0;
					}
					if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]==1){  //右下
						SegmentationFunction::SegmentationInsert(x+1,y+1);
						TMPWebcamBoolBuffer[(y+1) * VisionStatus::ImageWidth + (x+1)]=0;
					}
					s++;
				}  //抓到一個物體了!!!!
				if (temp<s)
				{
					temp=s;
					x1_temp=SegmentationFunction::Xmin;
					x2_temp=SegmentationFunction::Xmax;
					y1_temp=SegmentationFunction::Ymin;
					y2_temp=SegmentationFunction::Ymax;
				}
				
				
			}
		}
	}
	SegmentationFunction::Xmin=x1_temp;
	SegmentationFunction::Xmax=x2_temp;
	SegmentationFunction::Ymin=y1_temp;
	SegmentationFunction::Ymax=y2_temp;
	SegmentationFunction::DrawLine(WebcamBoolBuffer);
}
void SegmentationFunction::DrawLine(unsigned char *WebcamBuffer){  //畫框框
		
	for(int j=SegmentationFunction::Ymin; j < SegmentationFunction::Ymax; j++){
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmin)+2] = 255;  //紅線
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmin)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmin)+0] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmax)+2] = 255;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmax)+1] = 0;
		WebcamBuffer[3*(j * VisionStatus::ImageWidth + SegmentationFunction::Xmax)+0] = 0;
	}
	for(int i=SegmentationFunction::Xmin; i < SegmentationFunction::Xmax; i++){
		WebcamBuffer[3*(SegmentationFunction::Ymin * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(SegmentationFunction::Ymin * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(SegmentationFunction::Ymin * VisionStatus::ImageWidth + i)+0] = 0;
		WebcamBuffer[3*(SegmentationFunction::Ymax * VisionStatus::ImageWidth + i)+2] = 255;
		WebcamBuffer[3*(SegmentationFunction::Ymax * VisionStatus::ImageWidth + i)+1] = 0;
		WebcamBuffer[3*(SegmentationFunction::Ymax * VisionStatus::ImageWidth + i)+0] = 0;
	}
}
