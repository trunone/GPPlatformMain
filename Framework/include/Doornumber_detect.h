#ifndef _Doornumber_detect_H_
#define _Doornumber_detect_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "SegmentFunction.h"
#include "ColorCheck.h"
	
using namespace std;
//----------------------------

namespace Robot
{
	class Doornumber_detect : public VisionModule
	{

	private:
		Doornumber_detect();
		static Doornumber_detect* m_UniqueInstance;
		void Gray_binarize(unsigned char *source, unsigned char *graybuffer);
		void Segment(unsigned char * TMPWebcamBoolBuffer);
		void merge(unsigned char *graybuffer, unsigned char *door, unsigned char *mix);
		void background_check(unsigned char *mix, int *b,int *w);
	public:
		
		static Doornumber_detect* GetInstance() { return m_UniqueInstance; }
		
		~Doornumber_detect();
		void Initialize();
		void doornumber_check(Mat frame);
		void Process();
	};
}

#endif
