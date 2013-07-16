#ifndef _SEGMENTATIONFUNCTION_H_
#define _SEGMENTATIONFUNCTION_H_
#include "VisionStatus.h"
#include <vector>

using namespace std;
//----------------------------

namespace Robot
{
	class SegmentationFunction : public VisionModule
	{

	private:
		static SegmentationFunction* m_UniqueInstance;
		
		SegmentationFunction();

		static int Xmax, Xmin, Ymax, Ymin;
		static int PointCnt;
	public:
		struct SegmentLocation
		{
			int x, y;
		};
		static SegmentationFunction* GetInstance() { return m_UniqueInstance; }
		
		~SegmentationFunction();
		
		void SegmentationInit(int Xvalue, int Yvalue);
		void SegmentationInsert(int Xvalue, int Yvalue);
		vector <SegmentLocation> LocationList;
	
		Segment(unsigned char *TMPWebcamBoolBuffer,unsigned char *WebcamBoolBuffer);
		void DrawLine(unsigned char *WebcamBuffer);

		void Process();
	};
		
}

#endif

