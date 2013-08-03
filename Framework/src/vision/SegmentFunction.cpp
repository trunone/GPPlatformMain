#include "SegmentFunction.h"

using namespace Robot;
using namespace std;
SegmentFunction* SegmentFunction::m_UniqueInstance = new SegmentFunction();

SegmentFunction::SegmentFunction() {
}
SegmentFunction::~SegmentFunction() {
}


int Xmax(0);
int Xmin(0);
int Ymax(0);
int Ymin(0);
int PointCnt(0);



void SegmentFunction::SegmentationInit(int Xvalue, int Yvalue)  //初始push第一個為0
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
void SegmentFunction::SegmentationInsert(int Xvalue, int Yvalue)
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


