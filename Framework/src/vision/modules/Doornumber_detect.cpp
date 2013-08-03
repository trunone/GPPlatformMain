#include "Doornumber_detect.h"
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
		int temp=0,x1_temp,x2_temp,y1_temp,y2_temp;
		for(int i=1; i<VisionStatus::GetInstance()->GetInstance()->ImageWidth-1; i++){
			for(int j=1; j<VisionStatus::GetInstance()->GetInstance()->ImageHeight-1; j++){
				if(TMPWebcamBoolBuffer[j * VisionStatus::GetInstance()->GetInstance()->ImageWidth + i]==2){  
					
					PublicFunction::GetInstance()->SegmentationInit(i,j);
					TMPWebcamBoolBuffer[j * VisionStatus::GetInstance()->ImageWidth + i]=0;  //清除標記
					int s=0;
					while (s <= PublicFunction::GetInstance()->PointCnt){   //判斷是否抓完
						int x = PublicFunction::GetInstance()->LocationList[s].x,  //pop
							y = PublicFunction::GetInstance()->LocationList[s].y;
						if(x == 0 || y == 0){  //防止超值(避免掃入邊緣點)
							s++;
							continue;
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + (x-1)]==2){ //判斷周圍左上
							PublicFunction::GetInstance()->SegmentationInsert(x-1,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + (x-1)]=0;  //消除標記
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + x]==2){  //上
							PublicFunction::GetInstance()->SegmentationInsert(x,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + x]=0;
						}
						if(TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + (x+1)]==2){ //右上
							PublicFunction::GetInstance()->SegmentationInsert(x+1,y-1);
							TMPWebcamBoolBuffer[(y-1) * VisionStatus::GetInstance()->ImageWidth + (x+1)]=0;
						}
						if(TMPWebcamBoolBuffer[y * VisionStatus::GetInstance()->ImageWidth + (x-1)]==2){  //左
							PublicFunction::GetInstance()->SegmentationInsert(x-1,y);
							TMPWebcamBoolBuffer[y * VisionStatus::GetInstance()->ImageWidth + (x-1)]=0;
						}
						if(TMPWebcamBoolBuffer[y * VisionStatus::GetInstance()->ImageWidth + (x+1)]==2){  //右
							PublicFunction::GetInstance()->SegmentationInsert(x+1,y);
							TMPWebcamBoolBuffer[y * VisionStatus::GetInstance()->ImageWidth + (x+1)]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + (x-1)]==2){  //左下
							PublicFunction::GetInstance()->SegmentationInsert(x-1,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + (x-1)]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + x]==2){  //下
							PublicFunction::GetInstance()->SegmentationInsert(x,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + x]=0;
						}
						if(TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + (x+1)]==2){  //右下
							PublicFunction::GetInstance()->SegmentationInsert(x+1,y+1);
							TMPWebcamBoolBuffer[(y+1) * VisionStatus::GetInstance()->ImageWidth + (x+1)]=0;
						}
						s++;
					}  
					if (temp<s)
					{
						temp=s;
						x1_temp=PublicFunction::GetInstance()->Xmin;
						x2_temp=PublicFunction::GetInstance()->Xmax;
						y1_temp=PublicFunction::GetInstance()->Ymin;
						y2_temp=PublicFunction::GetInstance()->Ymax;
					}
					
					
				}
			}
		}
		PublicFunction::GetInstance()->Xmin=x1_temp;
		PublicFunction::GetInstance()->Xmax=x2_temp;
		PublicFunction::GetInstance()->Ymin=y1_temp;
		PublicFunction::GetInstance()->Ymax=y2_temp;
		
	}
void Doornumber_detect::merge(unsigned char *graybuffer	}
},unsigned char *door,unsigned char *mix){
	for(int WidthCnt = 0; WidthCnt < 640; WidthCnt++){
			for(int HeightCnt = 0; HeightCnt < 480; HeightCnt++){
				float temp=(graybuffer[HeightCnt * 640 + WidthCnt])/255+door[HeightCnt * 640 +WidthCnt];
				mix[(HeightCnt *640 + WidthCnt)]=temp;
			}		
	}
}
void Doornumber_detect::background_check(unsigned char *mix,int *b,int *w){
	for(int WidthCnt = (PublicFunction::GetInstance()->Xmin-5); WidthCnt <= (PublicFunction::GetInstance()->Xmax+5) ; WidthCnt++){
		for(int HeightCnt = (PublicFunction::GetInstance()->Ymin-5);HeightCnt<=(PublicFunction::GetInstance()->Ymax+5) ; HeightCnt++){
			if(mix[(HeightCnt *640 + WidthCnt)]==1){
				w=w+1;
			}else {
				if(mix[(HeightCnt *640 + WidthCnt)]==0){
				b=b+1;
				}
			}
		}

void Doornumber_detect::doornumber_check(Mat frame){
	cvtColor(frame, hsv, CV_BGR2HSV);
	unsigned char graybuffer[3*640*480];
	unsigned char Red_door[640*480];
	unsigned char Blue_door[640*480];
	unsigned char Green_door[640*480];
	unsigned char mix[640*480];
	int b=0,w=0;
	for(int WidthCnt = 0; WidthCnt < 640; WidthCnt++){
			for(int HeightCnt = 0; HeightCnt < 480; HeightCnt++){
						
				float hValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+0]*360/180;
				float sValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+1]*100/255;
				float vValue = hsv.data[3*(HeightCnt * 640 + WidthCnt)+2]*100/255;
				
				
				if(PublicFunction::GetInstance()->HSV_hsvCheckRange_Red(hValue, sValue, vValue)){					 
					Red_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Red_door[(HeightCnt * 640 + WidthCnt)] = 0;	
				}
				if(PublicFunction::GetInstance()->HSV_hsvCheckRange_Green(hValue, sValue, vValue)){
					Green_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Green_door[HeightCnt * 640 + WidthCnt] = 0;
				}
				if(PublicFunction::GetInstance()->HSV_hsvCheckRange_Blue(hValue, sValue, vValue)){
					Blue_door[(HeightCnt * 640 + WidthCnt)] = 2;
				}else{
					Blue_door[HeightCnt * 640 + WidthCnt] = 0;
				}				
			}
		}
	Gray_binarize(frame.data,graybuffer);
	merge(graybuffer,Red_door,mix);
	Segment(Red_door);
	
	if(PublicFunction::GetInstance()->Xmax>0
		&&PublicFunction::GetInstance()->Xmin>=0
		&&PublicFunction::GetInstance()->Ymax>0
		&&PublicFunction::GetInstance()->Ymin>=0){
		background_check(mix,b,w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::GetInstance()->door_status = 1; //red
		}else {
			b=0;
			w=0;
		}
	}
	merge(graybuffer,Green_door,mix);
	Segment(Green_door);
	if(PublicFunction::GetInstance()->Xmax>0
		&&PublicFunction::GetInstance()->Xmin>=0
		&&PublicFunction::GetInstance()->Ymax>0
		&&PublicFunction::GetInstance()->Ymin>=0){
		background_check(mix,b,w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::GetInstance()->door_status = 2; //green
		}else {
			b=0;
			w=0;
		}
	}
	merge(graybuffer,Blue_door,mix);
	Segment(Blue_door);
	if(PublicFunction::GetInstance()->Xmax>0
		&&PublicFunction::GetInstance()->Xmin>=0
		&&PublicFunction::GetInstance()->Ymax>0
		&&PublicFunction::GetInstance()->Ymin>=0){
		background_check(mix,b,w);
		if(b<w)
		{
			b=0;
			w=0;
			VisionStatus::GetInstance()->door_status = 3; //blue
		}else {
			b=0;
			w=0;
		}
	}
}
void Process(){
	doornumber_check(VisionStatus::GetInstance()->VideoFrame);
}
