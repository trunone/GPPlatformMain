/*
<<<<<<< HEAD
 *   Status.h
=======
 *   StrategyStatus.h
>>>>>>> Motion
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STATUS_H_
#define _STATUS_H_

//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include "Vector.h"
#include "Wheeled.h"
#include "Motors.h"

namespace Robot
{
	class Status
	{
	private:

	public:
		//static CvCapture* visioncapture;
		static int a;

        static double FI;
        static double w;
        static Vector3D vector;

        static Motors motors;
	};
}

#endif
