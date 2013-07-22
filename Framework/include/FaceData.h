/*
 *   FaceData.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _FACEDATA_H_
#define _FACEDATA_H_

#include <string.h>

#include "minIni.h"
#include "VisionModule.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define TASK_SECTION    "FaceData"

namespace Robot
{
	class FaceData : public VisionModule
	{
	private:
	
	public:
		void FaceData();
	};
}

#endif
