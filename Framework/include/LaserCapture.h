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
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

#define TASK_SECTION    "LaserCapture"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class LaserCapture : public LocationModule
	{
	private:
		static LaserCapture* m_UniqueInstance;

 		qrk::Urg_driver urg;   
		
		LaserCapture();
	public:
		static LaserCapture* GetInstance() { return m_UniqueInstance; }
		
		//static char b;

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
