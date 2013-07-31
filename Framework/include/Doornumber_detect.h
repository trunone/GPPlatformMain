#ifdef _Doornumber_detect_H_
#define _Doornumber_detect_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "SegmentationFunction.h"
#include "ColorModel.h"
 using namespace std;
//----------------------------

namespace Robot
{
	class Doornumber_detect : public VisionModule
	{

	private:
		static Doornumber_detect* m_UniqueInstance;
		
		Doornumber_detect();

		void Gray_binarize(unsigned char *source, unsigned char *graybuffer);
		void Segment(unsigned char * TMPWebcamBoolBuffer);
		void merge(unsigned char *graybuffer, unsigned char *door, unsigned char *mix);
		void background_check(unsigned char *mix, int *b,int *w);
	public:
		
		static Doornumber_detect* GetInstance() { return m_UniqueInstance; }
		
		~Doornumber_detect();
		
		void doornumber_check(Mat frame);
		void Process();
	};
}

#endif
