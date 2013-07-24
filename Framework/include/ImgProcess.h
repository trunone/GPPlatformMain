/*
 *   ImgProcess.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _FACEDATA_H_
#define _FACEDATA_H_

#include <string.h>
#include "VisionStatus.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Robot
{
	class ImgProcess
	{
	private:

	public:
		static void FaceData();
	};
}

#endif
