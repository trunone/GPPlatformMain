/*
 *   StrategyStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STRATEGY_STATUS_H_
#define _STRATEGY_STATUS_H_

#include "Vector.h"

namespace Robot
{
	class StrategyStatus
	{
	private:

	public:
        static long MotorSpeed[3];
        static double FI;
        static double w;
        static double x;
        static double y;
	};
}

#endif
