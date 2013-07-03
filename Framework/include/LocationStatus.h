/*
 *   LocationStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LOCATION_STATUS_H_
#define _LOCATION_STATUS_H_

#include "Vector.h"
#include <iostream>
#include <stdio.h>
#include "Status.h"
#include "LaserCapture.h"
#include "ReadLaser.h"
using namespace std;

namespace Robot
{
	class LocationStatus
	{
	private:

	public:
            static vector<long> data;
            static long time_stamp;
	};
}

#endif
