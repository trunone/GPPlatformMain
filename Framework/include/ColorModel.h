#ifndef _ColorModel_H_
#define _ColorModel_H_
#include "VisionStatus.h"
#include "SegmentationFunction.h"
#include <tinyxml.h>
using namespace std;
//----------------------------

namespace Robot
{
	class ColorModel : public VisionModule
	{
	

	private:
		
		static ColorModel* m_UniqueInstance;
		
		ColorModel();
		bool HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue);

		bool HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue);
		void Erosion(unsigned char *source, unsigned char *target);
		void Dilation(unsigned char *source, unsigned char *target);
	public:
		static ColorModel* GetInstance() { return m_UniqueInstance; }
		int loadxml(TiXmlElement* element);  //////////load xml
		~ColorModel();
		void Process();

		
	};
}
#endif
