//=============================================
//File name: Stra_Task
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/17
//
//Function :The room patrol strategy for sks
//===============================================

/*
 *   Task.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _Task_H_
#define _Task_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include "StrategyStatus.h"
#include "LocationStatus.h"
#include "StrategyModule.h"
#include "TCoordinate.h"

#define Def_AnglePrecision (5*M_PI/180.0)
#define Def_MinTurnAngle (20*M_PI/180.0)
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

    		etWaitDoorOpen,

		etMakeSoundMove		
		

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

    		bool MotionToPosition( TCoordinate  Goal );

    		bool TurnToAngle( float GoalAngle );
		
		bool SpecialMove( int Forward );

    		bool SpecialTurn();

    		bool WaitDoorOpen();

    		bool TouchButton();		

    		bool Backward();

		bool MakeSoundMove();		

    		//=============

    		void WaitCatchball();
		
		void MakeSound();		

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
		static TCoordinate vector;
		    
		};
}

#endif
