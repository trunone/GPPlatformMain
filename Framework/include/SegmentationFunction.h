#ifndef _SEGMENTATIONFUNCTION_H_
#define _SEGMENTATIONFUNCTION_H_
#include "VisionStatus.h"
#include <vector>

using namespace std;
//----------------------------

namespace Robot
{
	class SegmentationFunction
	{

	private:
		static int Xmax, Xmin, Ymax, Ymin;
		static int PointCnt;
	public:
		struct SegmentLocation
		{
			int x, y;
		};
		
		void SegmentationInit(int Xvalue, int Yvalue);
		void SegmentationInsert(int Xvalue, int Yvalue);
		vector <SegmentLocation> LocationList;
	
		SegmentationFunction(unsigned char *TMPWebcamBoolBuffer,unsigned char *WebcamBoolBuffer);
		DrawLine(unsigned char *WebcamBuffer);
	};
		
}

#endif

