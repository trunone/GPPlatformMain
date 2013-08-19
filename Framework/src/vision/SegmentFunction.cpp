#include "SegmentFunction.h"
#include <iostream>
using namespace Robot;
using namespace std;


SegmentFunction* SegmentFunction::m_UniqueInstance = new SegmentFunction();

SegmentFunction::SegmentFunction() {
}
SegmentFunction::~SegmentFunction() {
}






void SegmentFunction::SegmentationInit(int Xvalue, int Yvalue)
{
    SegmentLocation node;
    node.x = Xvalue;
    node.y = Yvalue;
    LocationList.push_back(node);
    VisionStatus::Xmax = Xvalue;
    VisionStatus::Xmin = Xvalue;
    VisionStatus::Ymax = Yvalue;
    VisionStatus::Ymin = Yvalue;
    VisionStatus::PointCnt = 0;
}
void SegmentFunction::SegmentationInsert(int Xvalue, int Yvalue)
{
    SegmentLocation node;
    node.x = Xvalue;
    node.y = Yvalue;
    LocationList.push_back(node);
    if(Xvalue > VisionStatus::Xmax)	VisionStatus::Xmax = Xvalue;
    if(Xvalue < VisionStatus::Xmin)	VisionStatus::Xmin = Xvalue;
    if(Yvalue > VisionStatus::Ymax)	VisionStatus::Ymax = Yvalue;
    if(Yvalue < VisionStatus::Ymin)	VisionStatus::Ymin = Yvalue;
    VisionStatus::PointCnt++;
}


