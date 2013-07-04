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
        static long Motor1Speed, Motor2Speed, Motor3Speed;
        static double FI;
        static double w;
        static Vector3D vector;
	};
}

#endif
