#ifdef _ColorCheck_H_
#define _ColorCheck_H_

#include "VisionStatus.h"


using namespace std;
//----------------------------

namespace Robot
{
	class ColorCheck 
	{

	private:
		static ColorCheck* m_UniqueInstance;
		
		ColorCheck();

		
	public:
		
		static ColorCheck* GetInstance() { return m_UniqueInstance; }
		
		~ColorCheck();
		
		bool HSV_hsvCheckRange_Blue(float hValue, float sValue, float vValue);
		bool HSV_hsvCheckRange_Red(float hValue, float sValue, float vValue);
		bool HSV_hsvCheckRange_Green(float hValue, float sValue, float vValue);

		
	};
}

#endif
