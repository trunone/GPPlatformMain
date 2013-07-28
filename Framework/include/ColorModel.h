#ifndef _ColorModel_H_
#define _ColorModel_H_
#include "VisionModule.h"

#include <tinyxml.h>

namespace Robot
{
	class ColorModel : public VisionModule
	{
	private:
		ColorModel();
		static ColorModel* m_UniqueInstance;

		bool HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue);
		bool HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue);
		bool HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue);

		void Erosion(unsigned char *source, unsigned char *target);
		void Dilation(unsigned char *source, unsigned char *target);
	public:
		static ColorModel* GetInstance() { return m_UniqueInstance; }
		~ColorModel();
        void Initialize();
		void Process();
		int LoadXMLSettings(TiXmlElement* element);  //////////load xml
	};
}
#endif
