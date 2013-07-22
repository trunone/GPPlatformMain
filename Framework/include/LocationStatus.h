/*
 *   LocationStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#include <vector>
#include <fstream>
#include <string.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
#include <TCoordinate.h>
#include <Normalrand.h>
#include "TCoordinate.h"
#include "Normalrand.h"
#include <tinyxml.h>
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
		struct Furniture
		{
			int x;
			int y;
			int Width;	
			int Height;
			int Angle;			
		};
		int loadxml (TiXmlElement* element);
		static Furniture LivingRM_Sofa,LivingRM_Table,LivingRM_Cabinet,DiningRM_Table,DiningRM_Chair,Library_Desk,Library_Chair,Library_Cabinet,BedRM_Bed,DiningRM_door,BedRM_door,ChargeArea,EndArea;
		static float ScanLineData[24];
		static bool FlagEvaluatuonEnable;		
		static bool FlagCoerceEvaluatuon;
		static TCoordinate Position;
		static double Handle;
		static bool FlagNewFeedback;
		static bool enable;

	};


}

#endif
