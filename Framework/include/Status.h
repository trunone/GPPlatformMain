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

#include "Vector.h"
#include "Wheeled.h"
#include "TCoordinate.h"
namespace Robot
{
	class Status
	{
	private:

	public:
		static int a;

        static double FI;
        static double w;
        static Vector3D vector;
	
	};
}

#endif
