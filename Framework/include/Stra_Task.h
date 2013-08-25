/*
 *   Stra_Task.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _STRA_TASK_H_
#define _STRA_TASK_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include "StrategyStatus.h"
#include "LocationStatus.h"
#include "StrategyModule.h"
#include "TCoordinate.h"
#include <tinyxml.h>

#define Def_AnglePrecision (5*M_PI/180.0)
#define Def_MinTurnAngle (20*M_PI/180.0)
using namespace std;

namespace Robot
{
class Stra_Task : public StrategyModule
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

    typedef enum {

        etGRAStar = 0,

        etGRFaceDoor

    } teGeneralRoomState;

private:
    static Stra_Task* m_UniqueInstance;

    //static Vector3D vector;

    Stra_Task();

    void SetAStar( TCoordinate  Goal );

    void ActiveFunction();

    //============= Active Function ==================

    bool MotionToPosition( TCoordinate  Goal );

    bool TurnToAngle( float GoalAngle );

    bool TouchButton();

    //=============

    void WaitCatchball();

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

    bool DoorState;     //true: open, false: close

    int TouchCnt;

    int *PastScanLineData;

    int LivRM;

    int DinRM;

    int Lib;

    int BedRM;

public:
    //---------loadxml----------
    int LoadXMLSettings(TiXmlElement* element);

    static Stra_Task* GetInstance() {
        return m_UniqueInstance;
    }

    ~Stra_Task();

    void Initialize();

    void Process();

    static TCoordinate vector;

};
}

#endif
