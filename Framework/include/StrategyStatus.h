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
#include "Stra_Task.h"

namespace Robot
{
	class StrategyStatus
	{
	private:

	public:
		//motion data        	
		static long Motor1Speed, Motor2Speed, Motor3Speed;
        	static double FI;
 	        static double x;
       		static double y;
		static double w;
        	static Vector3D vector;
		static TCoordinate StartPosition;
		static TCoordinate EscapePosition;
	        static TCoordinate EndPosition; 
		// task information
		typedef enum{ etFree = 0, etOpen, etClosed, etObstacle, etFocus }teNodeStatus;
		
		typedef enum{ etSKSMoving = 0, etSKSCatchBall, etSKSMakeSound, etCatchFinish }teSKSState;
		
		typedef enum{ etMotion =0, etAchieve }teAstarStatus;

	        typedef struct{ int Status;
				TCoordinate StartPos;
				TCoordinate GoalPos;
				int PCnt; 
			      }tsAStarPath;
			
		typedef struct{ TCoordinate Origin;
				float Scale;
				int Width,Height;
			      }tsMapInfo;
		
		typedef struct{ bool Enable; 
			        TCoordinate Door; 
				TCoordinate Center;
			        TCoordinate LeftBottom;
				TCoordinate RightTop; 
				short BallDirection; 
			      }tsRoom;
		
		typedef struct{ tsRoom Info[5]; int Cnt; teSKSState SKSRoomState; }tsRoomInfo;
		
		typedef struct{ TCoordinate Position; float Angle; int Distance;
				short UPDown;       //Down : 1 , Up : -1 , Mid : 0 , No ball :-999
				short LeftRight;   //Left : 1 , URightp : -1 , Mid : 0 , No ball :-999
				unssigned char FindBallCnt; }tsStraBallInfo;
		//TStra(void);

        	//~TStra(void);



        	void Initial();

                static	bool FlagRoomRenew;

        	static tsRoomInfo Room;
		static int ThiefRoom;
        	static int RoomSort;		

        	static int FindBallEn;		

        	static int LivRM, DinRM, Lib, BedRM;
		static tsAStarPath AStarPath;
		 //---------- Strategy -----------------------------------



        	static TCoordinate Goal1;                   // Target1 vector

        	static TCoordinate Goal2;                   // Target2 vector

        	static double Direction;                    // Target direction



        	static bool FlagDetour;                     //true: Ÿaªñ¥ØŒÐÂIP2 , flase: »·Â÷¥ØŒÐÂIp2

        	static bool FlagForward;                     

        	//----------------------------------------------------------------------



        	//----------- Path Plan ------------------------------------------------

        	static TCoordinate GoalVector;

        	//----------------------------------------------------------------------



        	//----------- Avoidance ------------------------------------------------

        	static bool FlagAvoidEnable;

        	static TCoordinate CorrectionVector;

        	//----------------------------------------------------------------------



        	//----------- Velocity Control -----------------------------------------

        	static float FixSpeed;                                 // Speed Power 1~100 %

        	static double MotionDistance;		//²Ÿ°Ê¶ZÂ÷

        	static double MotionAngle;		//²Ÿ°Êš€«×



        	static TCoordinate PathMotion;

        	static float PathRotation;

        	//----------------------------------------------------------------------

        	static bool PathR_Priority;

        	//-----------------------------



        	//----------- Ball Data -----------------------------------------

        	static tsStraBallInfo RadBallInfo;

                static int AX12_Angle;

        	//tsObjectiveInfo RedBall;

        	static unsigned char FindBallCnt;

        	static bool FlagRecognize;



        	//---------- Laser Average Data-----------------

		static int LaserAverageData[3];       //¹p®g¥ª ¥k «e ¥­§¡Data



        	//---------- ŽM²yµŠ²€šBÆJ-----------------

        	static int FindBallState;



        	//---------- ŽM²y­«·sÅxÂI-----------------

        	static bool FindLocResample;



        	static TCoordinate RseLocPos;        //·sªº©wŠìÂI	

	};
}

#endif
