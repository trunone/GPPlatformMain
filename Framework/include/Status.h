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

namespace Robot
{
	class Status
	{
	private:

	public:
		static CvCapture* visioncapture;
		static int a;
	};
}

#endif
