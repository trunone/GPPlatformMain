/*
 *   VisionCapture.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _VISIONCAPTURE_H_
#define _VISIONCAPTURE_H_

#include <string.h>

#include "minIni.h"
#include "VisionModule.h"

#ifdef ENABLE_OPENCV
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

#define TASK_SECTION    "VisionCapture"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class VisionCapture : public VisionModule
	{
	private:
		static VisionCapture* m_UniqueInstance;

		VisionCapture();

	public:
		static VisionCapture* GetInstance() { return m_UniqueInstance; }
		
		static char a;

		~VisionCapture();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
