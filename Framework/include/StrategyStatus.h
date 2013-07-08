/*
 *   StrategyStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STRATEGY_STATUS_H_
#define _STRATEGY_STATUS_H_

#include "TCoordinate.h"
#include "Vector.h"
#include <vector>
#include "Task.h"

namespace Robot
{
	class StrategyStatus
	{
	private:

	public:
		//motion data        	
		static long Motor1Speed, Motor2Speed, Motor3Speed;
        	static double FI;
       		static double w;
        	static Vector3D vector;
		// task information
		typedef enum{ etFree = 0, etOpen, etClosed, etObstacle, etFocus }teNodeStatus;
		
		typedef enum{ etSKSMoving = 0, etSKSCatchBall, etCatchFinish }teSKSState;
		
		typedef enum{ etMotion =0, etAchieve }teAstarStatus;

		typedef struct{ int Status;
				Vector3D StartPos;
				Vector3D GoalPos;
				int PCnt; 
			      }tsAStarPath;
			
		typedef struct{ Vector3D Origin;
				float Scale;
				int Width,Height;
			      }tsMapInfo;
		
		typedef struct{ bool Enable; 
				Vector3D Door; 
				Vector3D Center;
				Vector3D LeftBottom;
				Vector3D RightTop; 
				short BallDirection; 
			      }tsRoom;
		
		typedef struct{ tsRoom Info[5]; int Cnt; teSKSState SKSRoomState; }tsRoomInfo;
		
		typedef struct{ Vector3D Position; float Angle; int Distance;
				short UPDown;       //Down : 1 , Up : -1 , Mid : 0 , No ball :-999
				short LeftRight;   //Left : 1 , URightp : -1 , Mid : 0 , No ball :-999
				unsigned char FindBallCnt; }tsStraBallInfo;
		//TStra(void);

        	//~TStra(void);



        	void Initial();

        	bool FlagRoomRenew;

        	tsRoomInfo Room;

        	int RoomSort;		

        	int FindBallEn;		

        	int LivRM, DinRM, Lib, BedRM;
		tsAStarPath AStarPath;
		 //---------- Strategy -----------------------------------



        	TCoordinate Goal1;                   // Target1 vector

        	TCoordinate Goal2;                   // Target2 vector

        	double Direction;                    // Target direction



        	bool FlagDetour;                     //true: Ÿaªñ¥ØŒÐÂIP2 , flase: »·Â÷¥ØŒÐÂIp2

        	bool FlagForward;                     

        	//----------------------------------------------------------------------



        	//----------- Path Plan ------------------------------------------------

        	TCoordinate GoalVector;

        	//----------------------------------------------------------------------



        	//----------- Avoidance ------------------------------------------------

        	bool FlagAvoidEnable;

        	TCoordinate CorrectionVector;

        	//----------------------------------------------------------------------



        	//----------- Velocity Control -----------------------------------------

        	float FixSpeed;                                 // Speed Power 1~100 %

        	double MotionDistance;		//²Ÿ°Ê¶ZÂ÷

        	double MotionAngle;		//²Ÿ°Êš€«×



        	TCoordinate PathMotion;

        	float PathRotation;

        	//----------------------------------------------------------------------

        	bool PathR_Priority;

        	//-----------------------------



        	//----------- Ball Data -----------------------------------------

        	tsStraBallInfo RadBallInfo;



        	//tsObjectiveInfo RedBall;

        	unsigned char FindBallCnt;

        	bool FlagRecognize;



        	//---------- Laser Average Data-----------------

        	int LaserAverageData[3];       //¹p®g¥ª ¥k «e ¥­§¡Data



        	//---------- ŽM²yµŠ²€šBÆJ-----------------

        	int FindBallState;



        	//---------- ŽM²y­«·sÅxÂI-----------------

        	bool FindLocResample;



        	TCoordinate RseLocPos;        //·sªº©wŠìÂI	
	};
}

#endif
