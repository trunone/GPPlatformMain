/*
 *   LaserCapture.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _LASERCAPTURE_H_
#define _LASERCAPTURE_H_

#include <string.h>

#include "minIni.h"
#include "LocationModule.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define TASK_SECTION    "LaserCapture"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class LaserCapture : public LocationModule
	{
	private:
		static LaserCapture* m_UniqueInstance;
		
		LaserCapture();

	public:
		static LaserCapture* GetInstance() { return m_UniqueInstance; }
		
		static char a;

		~LaserCapture();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
