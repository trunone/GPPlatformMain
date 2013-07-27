//=============================================
//File name: Stra_FindBall
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/09
//
//Function :Find Ball strategy
//===============================================
#ifndef Stra_FindBallH
#define Stra_FindBallH

#include "StrategyModule.h"
#include "StrategyStatus.h"
#include "LocationStatus.h"
#include <time.h>
#include "TCoordinate.h"
namespace Robot{
	class Stra_FindBall : public StrategyModule
	{

	typedef enum
	{
    		etInitial = 0,

    		etEnterRoom,

    		etSearchBall,

    		etTurnToBall,

    		etApproachBall,

    		etEatBall,

    		etGotoCenter,

    		etRoom2,

    		etLeaveRoom

	}teFindBallState;

	typedef enum
	{

    		etLeftRight=0,

    		etRoomMid,

    		etNearSearch,

    		etEndSearch

	}teSearchStep;

	public:

		static Stra_FindBall*  GetInstance() {return m_UniqueInstance;}
    		
    		~Stra_FindBall();   //žÑºcšçŠ¡

    		//virtual string  ParameterReset(void);           //°ÑŒÆ­«žm

    		void Initialize(void);                  //ªì©l€Æ

    		void Process(void);                     //°õŠæšçŒÆ

	private:

	        static Stra_FindBall* m_UniqueInstance; 
		
		Stra_FindBall();

    		void  RenewRoomCorner();

   		int  MiddleValue(int *Array, int Num);

    		void  AnalyseBall();

    		int  DisTransfer(int AX12_CMD);

    		void  AX12_TakeBall();

    		void  SearchBall_Initial();

    		void  SearchBall();

    		void  TurnToBall();

    		void  Approach_Ball();

    		bool  Eat_Ball();

    		void  DetermineEatDirection();

    		teFindBallState FindBallState;

    		teSearchStep SearchStep;

    		int CenterThreshold;

    		short BallDirection;

    		short EatBallDirection;

    		TCoordinate PastOdometer;

    		int RoomCnt;

    		TCoordinate RoomVector;

    		TCoordinate CenterVector;

    		TCoordinate Corner[4];

    		int CornerID;

    		bool FlagEatBall;

    		bool FlagRecognize;

    		short LeftRightSearch;     // ¥ª : 1, ¥k :-1 , €€¶¡:0

    		float LoseBallAngle;

    		int LoseBallCnt;

    		int SearchTrunStep;

    		bool Lock;

    		int SearchTimes;

    		int *PastScanLineData;

		int Room2Step;

	};
}

#endif


