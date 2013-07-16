#ifndef _Colormodel_H_
#define _Colormodel_H_
#include "VisionStatus.h"
#include "SegmentationFunction.h"

using namespace std;
//----------------------------

namespace Robot
{
	class Colormodel : public VisionModule
	{
	

	private:
		
		static Colormodel* m_UniqueInstance;
		
		Colormodel();
		bool HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue);
		void Erosion(unsigned char *source, unsigned char *target);
		void Dilation(unsigned char *source, unsigned char *target);
	public:
		static Colormodel* GetInstance() { return m_UniqueInstance; }
		
		~Colormodel();
		void Process();

		
	};
}
#endif
