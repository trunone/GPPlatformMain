#ifndef _Segmentation_H_
#define _Segmentation_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "SegmentFunction.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



using namespace std;
//----------------------------

namespace Robot
{
	class Segmentation : public VisionModule
	{

	private:
		static Segmentation* m_UniqueInstance;
		
		Segmentation();

		
	public:
		
		static Segmentation* GetInstance() { return m_UniqueInstance; }
		
		~Segmentation();
		static int Xcenter,Ycenter;
		
	
		void Segment(unsigned char *TMPWebcamBoolBuffer,unsigned char *WebcamBoolBuffer);
		void DrawLine(unsigned char *WebcamBuffer);

		void Process();
	};
		
}

#endif

