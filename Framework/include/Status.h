/*
 *   Status.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STATUS_H_
#define _STATUS_H_

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

namespace Robot
{
	class Status
	{
	private:

	public:
		static CvCapture* visioncapture;
		static int a;

		static int b;

	};
}

#endif
