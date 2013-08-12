#ifndef _ColorModel_H_
#define _ColorModel_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "ColorCheck.h"
#include <tinyxml.h>

namespace Robot
{
	class ColorModel : public VisionModule
	{
	private:
		ColorModel();
		static ColorModel* m_UniqueInstance;
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
