#ifndef _SEGMENTATIONFUNCTION_H_
#define _SEGMENTATIONFUNCTION_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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

		
	public:
		struct SegmentLocation
		{
			int x, y;
		};
		static SegmentationFunction* GetInstance() { return m_UniqueInstance; }
		
		~SegmentationFunction();
		static int Xcenter,Ycenter;
		static int Xmax, Xmin, Ymax, Ymin;
		static int PointCnt;
		void SegmentationInit(int Xvalue, int Yvalue);
		void SegmentationInsert(int Xvalue, int Yvalue);
		vector <SegmentLocation> LocationList;
	
		void Segment(unsigned char *TMPWebcamBoolBuffer,unsigned char *WebcamBoolBuffer);
		void DrawLine(unsigned char *WebcamBuffer);

		void Process();
	};
		
}

#endif

