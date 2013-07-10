/*
 *   LocationStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
#include <TCoordinate.h>
#include <Normalrand.h>

#ifndef _LOCATION_STATUS_H_
#define _LOCATION_STATUS_H_

#define Def_InitValue -9999
#define Def_DuToPI 0.0174533
#define Def_PIToDu 57.29578
#define Def_RobotRadius 25
#define Def_LaserGap 10

using namespace std;

namespace Robot
{
	class LocationStatus
	{

	private:

	public:

        float ScanLineData[24];

        bool FlagEvaluatuonEnable;		

        bool FlagCoerceEvaluatuon;

        TCoordinate Position;

        double Handle;
	

	};
}

#endif
