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
	typedef struct{
		TCoordinate Position;
		float Direction;
	}FeedBack_Info;

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
		
		static Furniture LivingRM_Sofa;
		static Furniture LivingRM_Table;
		static Furniture LivingRM_Cabinet;
		static Furniture DiningRM_Table;
		static Furniture DiningRM_Chair;
		static Furniture Library_Desk;
		static Furniture Library_Chair;
		static Furniture Library_Cabinet;
		static Furniture BedRM_Bed;
		static Furniture DiningRM_door;
		static Furniture BedRM_door;
		static Furniture ChargeArea;
		static Furniture EndArea;

		static float ScanLineData[24];
		static bool FlagEvaluatuonEnable;		
		static bool FlagCoerceEvaluatuon;
		static TCoordinate Position;
		static double Handle;
		static bool FlagNewFeedback;
		static bool FlagRenew;
		static bool FlagLaserBusy;
		static bool FlagLocInit;
		static bool enable;
		static TCoordinate SetRobotPosition;
		static double SetRobotHandle;
	    	static TCoordinate LaserGap;	// tmp. store for partilcefilter, wait for the truly laser information get will replace	
		static FeedBack_Info FB_Movement;

	};


}

#endif
