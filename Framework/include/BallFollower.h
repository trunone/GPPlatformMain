/*
 *   BallFollower.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _BALL_FOLLOWER_H_
#define _BALL_FOLLOWER_H_

#include "Point.h"
#include "BallTracker.h"


namespace Robot
{
	class BallFollower
	{
	private:
		int m_NoBallMaxCount;
		int m_NoBallCount;

	protected:

	public:
		bool DEBUG_PRINT;

		BallFollower();
		~BallFollower();

		void Process(Point2D ball_pos);
	};
}

#endif
