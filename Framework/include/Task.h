/*
 *   Task.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _Task_H_
#define _Task_H_

#include <string.h>
#include "Vector.h"
#include <vector>
#include "StrategyStatus.h" 
#include "StrategyModule.h"
#include "TCoordinate.h"

using namespace std;

namespace Robot
{
	class Task : public StrategyModule
	{
	typedef enum
	{

    		etIdle  =0,

    		etAStar,

    		etTurnToAngle,		

    		etMotionToPos,

    		etSpecialMove,

    		etSpecialTurn,

    		etTouchButton,

    		etBackward,

    		etWaitDoorOpen

	}teAvtiveState;

	typedef enum{

    		etGRAStar = 0,

    		etGRFaceDoor

		}teGeneralRoomState;
	private:
		static Task* m_UniqueInstance;
		//static Vector3D vector;
		
		Task();
		void SetAStar( TCoordinate  Goal );
		
		void ActiveFunction();
		
		//============= Active Function ==================

    		bool MotionToPosition( Vector3D  Goal );

    		bool TurnToAngle( float GoalAngle );
		
		bool SpecialMove( int Forward );

    		bool SpecialTurn();

    		bool WaitDoorOpen();

    		bool TouchButton();		

    		bool Backward();		

    		//=============

    		void WaitCatchball();		

    		void SpecialRoom3();

    		//TCoordinate Room3StartPos;

    		int Past_RoomCnt;

    		bool FlagSetInitialData;		

    		bool FlagTaskFinish;		

    		int ActiveState;

    		int GotoRoomStep;

    		float GoalAngle;		

    		TCoordinate StartPos,GoalPos;		

		float SpecialDistance;

    		int Past_LaserData;

    		bool DoorState;     //true: open, false: close

    		int TouchCnt;		

    		int *PastScanLineData;

	public:
		static Task* GetInstance() { return m_UniqueInstance; }
		
		~Task();

		void Initialize();
		void Process();
		static Vector3D vector;
		    
		};
}

#endif
