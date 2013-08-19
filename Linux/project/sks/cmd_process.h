#ifndef _DXL_MANAGER_CMD_PROCESS_H_
#define _DXL_MANAGER_CMD_PROCESS_H_

#include "LinuxWheeled.h"

#define PROGRAM_VERSION	"v1.00"
#define SCREEN_COL		80

// Position of Column
#define CMD_COL			2
#define PARAM_COL		27

// Position of Row
enum {
    LOCATION_THREAD_ROW = 1,
    STRATEGY_THREAD_ROW,
    VISION_THREAD_ROW,
    MOVE_VELOCITY_ROW,
    ROTATE_VELOCITY_ROW,
    PITCH_OFFSET_ROW,
    YAW_OFFSET_ROW,
    HIP_PITCH_OFFSET_ROW,
    AUTO_BALANCE_ROW,
    PERIOD_TIME_ROW,
    DSP_RATIO_ROW,
    STEP_FORWARDBACK_RATIO_ROW,
    STEP_FORWARDBACK_ROW,
    STEP_RIGHTLEFT_ROW,
    STEP_DIRECTION_ROW,
    TURNING_AIM_ROW,
    FOOT_HEIGHT_ROW,
    SWING_RIGHTLEFT_ROW,
    SWING_TOPDOWN_ROW,
    PELVIS_OFFSET_ROW,
    ARM_SWING_GAIN_ROW,
    BAL_KNEE_GAIN_ROW,
    BAL_ANKLE_PITCH_GAIN_ROW,
    BAL_HIP_ROLL_GAIN_ROW,
    BAL_ANKLE_ROLL_GAIN_ROW,
    P_GAIN_ROW,
    I_GAIN_ROW,
    D_GAIN_ROW,
    EMPTY_ROW,
    CMD_ROW,
    SCREEN_ROW
};

int _getch();
bool AskSave();


// Move cursor
void GoToCursor(int col, int row);
void MoveUpCursor();
void MoveDownCursor();
void MoveLeftCursor();
void MoveRightCursor();

// Disp & Drawing
void DrawIntro();
void DrawEnding();
void DrawScreen();
void ClearCmd();
void PrintCmd(const char *message);

// Edit value
void IncreaseValue(bool large);
void DecreaseValue(bool large);

// Command process
void BeginCommandMode();
void EndCommandMode();
void HelpCmd();
void SaveCmd();
void MonitorCmd();


#endif
