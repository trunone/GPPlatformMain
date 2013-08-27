/*
 *   Stra_Task_stage1.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _Stra_Task_stage1_H_
#define _Stra_Task_stage1_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include "StrategyStatus.h"
#include "LocationStatus.h"
#include "StrategyModule.h"
#include "TCoordinate.h"
#include <tinyxml.h>

#define MEMBERS 4

#define Def_AnglePrecision (5*M_PI/180.0)
#define Def_MinTurnAngle (20*M_PI/180.0)
using namespace std;

namespace Robot
{
class Stra_Task_stage1 : public StrategyModule
{
    typedef enum
    {

        etIdle = 0,

        etAStar,

        etTurnToAngle,

        etMotionToPos,

        etSpecialMove,

        etSpecialTurn,

        etTouchButton,

        etBackward,

        etWaitDoorOpen,

        etMakeSoundMove

    } teAvtiveState;

    typedef enum { etSKSStop =0, etSKSEatBall ,etSKSThrowBall } teSKSBallState;

    typedef enum { etSKSMoving = 0, etSKSCatchBall, etSKSMakeSound, etCatchFinish } teSKSState;

    typedef struct {
        bool Enable;
        TCoordinate Door;
        TCoordinate Center;
        TCoordinate LeftBottom;
        TCoordinate RightTop;
        short BallDirection;
    } tsRoom;

    typedef enum { etLivRM =0, etDinRM, etLib, etBedRM } teRoom;

    typedef struct {
        tsRoom Info[5];
        teRoom SortList[4];
        int Cnt;
        teSKSState SKSRoomState;
    } tsRoomInfo;

    tsRoomInfo Room;

    typedef enum {

        etGRAStar = 0,

        etGRFaceDoor

    } teGeneralRoomState;

    typedef struct {
        TCoordinate FrontPosition;
        TCoordinate MemberPosition;
        teRoom Room;
    } tsMember;

    tsMember Members[MEMBERS];

private:
    static Stra_Task_stage1* m_UniqueInstance;

    Stra_Task_stage1();

    void SetAStar( TCoordinate  Goal );

    void ActiveFunction();

    //============= Active Function ==================

    bool MotionToPosition( TCoordinate  Goal );

    bool TurnToAngle( float GoalAngle );

    bool TouchButton();

    bool Backward();

    //=============

    void EncounterPeople();

    void MakeSound();

    int Past_RoomCnt;

    bool FlagSetInitialData;

    bool FlagTaskFinish;

    int ActiveState;

    int GotoRoomStep;

    float GoalAngle;

    TCoordinate StartPos,GoalPos;

    float SpecialDistance;

    int Past_LaserData;

    int TouchCnt;

    TCoordinate RoomVector;

    TCoordinate CenterVector;

    int MemberIndex;

    double TouchDist;

public:
    //---------loadxml----------
    int LoadXMLSettings(TiXmlElement* element);

    static Stra_Task_stage1* GetInstance() {
        return m_UniqueInstance;
    }

    ~Stra_Task_stage1();

    void Initialize();

    void Process();

    static TCoordinate vector;

};
}

#endif
