#include "cmd_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <term.h>
#include <ncurses.h>

using namespace Robot;

int Col = PARAM_COL;
int Row = LOCATION_THREAD_ROW;
int Old_Col;
int Old_Row;
bool bBeginCommandMode = false;
bool bEdited = false;
int giMoveVelocity = 150;
double gdRotateVelocity = 0.3;


int _getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

struct termios oldterm, new_term;
void set_stdin(void)
{
    tcgetattr(0,&oldterm);
    new_term = oldterm;
    new_term.c_lflag &= ~(ICANON | ECHO | ISIG);
    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_term);
}

void reset_stdin(void)
{
    tcsetattr(0, TCSANOW, &oldterm);
}

bool AskSave()
{
    if(bEdited == true)
    {
        PrintCmd("Are you sure? (y/n)");
        if(_getch() != 'y')
        {
            ClearCmd();
            return true;
        }
    }

    return false;
}


void GoToCursor(int col, int row)
{
    char *cursor;
    char *esc_sequence;
    cursor = tigetstr("cup");
    esc_sequence = tparm(cursor, row, col);
    putp(esc_sequence);

    Col = col;
    Row = row;
}

void MoveUpCursor()
{
    if(Col == PARAM_COL)
    {
        if(Row > 0)
            GoToCursor(Col, Row-1);
    }
}

void MoveDownCursor()
{
    if(Col == PARAM_COL)
    {
        if(Row < CMD_ROW - 1)
            GoToCursor(Col, Row+1);
    }
}

void MoveLeftCursor()
{
}

void MoveRightCursor()
{
}

void DrawIntro()
{
    /*
    int n = 0;
    int param[JointData::NUMBER_OF_JOINTS * 5];
    int wGoalPosition, wStartPosition, wDistance;

    for(int id=JointData::ID_R_SHOULDER_PITCH; id<JointData::NUMBER_OF_JOINTS; id++)
    {
    	wStartPosition = MotionStatus::m_CurrentJoints.GetValue(id);
    	wGoalPosition = Walking::GetInstance()->m_Joint.GetValue(id) + MotionManager::GetInstance()->m_Offset[id];
    	if( wStartPosition > wGoalPosition )
    		wDistance = wStartPosition - wGoalPosition;
    	else
    		wDistance = wGoalPosition - wStartPosition;

    	wDistance >>= 2;
    	if( wDistance < 8 )
    		wDistance = 8;

    	param[n++] = id;
    	param[n++] = CM730::GetLowByte(wGoalPosition);
    	param[n++] = CM730::GetHighByte(wGoalPosition);
    	param[n++] = CM730::GetLowByte(wDistance);
    	param[n++] = CM730::GetHighByte(wDistance);
    }

    cm730->SyncWrite(MX28::P_GOAL_POSITION_L, 5, JointData::NUMBER_OF_JOINTS - 1, param);

    */
    int nrows, ncolumns;
    setupterm(NULL, fileno(stdout), (int *)0);
    nrows = tigetnum("lines");
    ncolumns = tigetnum("cols");

    system("clear");
    printf("\n");
    printf("[General Purpose Platform Controller %s]\n", PROGRAM_VERSION);
    printf("\n");
    printf(" *Terminal screen size must be %d(col)x%d(row).\n", SCREEN_COL, SCREEN_ROW);
    printf(" *Current terminal has %d columns and %d rows.\n", ncolumns, nrows);
    printf("\n");
    printf("\n");
    printf("Press any key to start program...\n");
    _getch();

    DrawScreen();
}

void DrawEnding()
{
    system("clear");
    printf("\n");
    printf("Exit!");
    printf("\n");
}

void DrawScreen()
{
    int old_col = Col;
    int old_row = Row;

    system("clear");
    GoToCursor(0, 0);

    // Display menu
    //      012345678901234567890123456789012345678901234567890123456789
    printf("                                                 ------------------------------\n"); // 0
    printf("Location Thread            **                    ---                        ---\n"); // 1
    printf("Strategey Thread           **                    --------------  --------------\n"); // 2
    printf("Vision Thread              **                    --------------  --------------\n"); // 3
    printf("Robot Move Velocity        **                    --------------  --------------\n"); // 4
    printf("Robot Rotate Velocity      **                    --------------  --------------\n"); // 5
    printf("Pitch(y) offset(degree)    **                    --------------  --------------\n"); // 6
    printf("Yaw(z) offset(degree)      **                    --------------  --------------\n"); // 7
    printf("Hip pitch offset(degree)   **                    --------------  --------------\n"); // 8
    printf("Auto balance(on/off)       **                    --------------  --------------\n"); // 9
    printf("Period time(msec)          **                    ---  -----   -----------------\n"); // 0
    printf("DSP ratio                  **                    ---  ----   ------------------\n"); // 1
    printf("Step forward/back ratio    **                    ---  ---   -------------------\n"); // 2
    printf("Step forward/back(mm)      **                    ---  --   --------------------\n"); // 3
    printf("Step right/left(mm)        **                    ---     ----------------------\n"); // 4
    printf("Step direction(degree)     **                    ---  -   ---------------------\n"); // 5
    printf("Turning aim(on/off)        **                    ---  ---   -------------------\n"); // 6
    printf("Foot height(mm)            **                    ---  ----   ------------------\n"); // 7
    printf("Swing right/left(mm)       **                    ---  -----   -----------------\n"); // 8
    printf("Swing top/down(mm)         **                    ------------------------------\n"); // 9
    printf("Pelvis offset(degree)      **                    ---------------  --------  ---\n"); // 0
    printf("Arm swing gain             **                    ---------------  --------  ---\n"); // 1
    printf("Balance knee gain          **                    ---------------  --------  ---\n"); // 2
    printf("Balance ankle pitch gain   **                    ---------------  --------  ---\n"); // 3
    printf("Balance hip roll gain      **                    ---------------  --------  ---\n"); // 4
    printf("Balance ankle roll gain    **                    ---------------  --------  ---\n"); // 5
    printf("P gain                     **                    ---------------- -------- ----\n"); // 6
    printf("I gain                     **                    ----------------- ------ -----\n"); // 7
    printf("D gain                     **                    -------------------    -------\n"); // 8
    printf("                                                 ------------------------------\n"); // 9
    ClearCmd();

    GoToCursor(PARAM_COL, LOCATION_THREAD_ROW);
    if(LocationManager::GetInstance()->IsRunning() == true)
        printf("ON     ");
    else
        printf("OFF    ");

    GoToCursor(PARAM_COL, STRATEGY_THREAD_ROW);
    if(StrategyManager::GetInstance()->IsRunning() == true)
        printf("ON     ");
    else
        printf("OFF    ");

    GoToCursor(PARAM_COL, VISION_THREAD_ROW);
    if(VisionManager::GetInstance()->IsRunning() == true)
        printf("ON     ");
    else
        printf("OFF    ");


    GoToCursor(PARAM_COL, MOVE_VELOCITY_ROW);
    printf("%d    ", giMoveVelocity);

    GoToCursor(PARAM_COL, ROTATE_VELOCITY_ROW);
    printf("%.1f    ", gdRotateVelocity);

    GoToCursor(PARAM_COL, PITCH_OFFSET_ROW);
    //printf("%.1f    ", Walking::GetInstance()->P_OFFSET);

    GoToCursor(PARAM_COL, YAW_OFFSET_ROW);
    //printf("%.1f    ", Walking::GetInstance()->A_OFFSET);

    GoToCursor(PARAM_COL, HIP_PITCH_OFFSET_ROW);
    //printf("%.1f    ", Walking::GetInstance()->HIP_PITCH_OFFSET);

    GoToCursor(PARAM_COL, AUTO_BALANCE_ROW);
    //if(Walking::GetInstance()->BALANCE_ENABLE == true)
    if(true)
        printf("ON     ");
    else
        printf("OFF    ");

    GoToCursor(PARAM_COL, PERIOD_TIME_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->PERIOD_TIME);

    GoToCursor(PARAM_COL, DSP_RATIO_ROW);
    //printf("%.2f    ", Walking::GetInstance()->DSP_RATIO);

    GoToCursor(PARAM_COL, STEP_FORWARDBACK_RATIO_ROW);
    //printf("%.2f    ", Walking::GetInstance()->STEP_FB_RATIO);

    GoToCursor(PARAM_COL, STEP_FORWARDBACK_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->X_MOVE_AMPLITUDE);

    GoToCursor(PARAM_COL, STEP_RIGHTLEFT_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->Y_MOVE_AMPLITUDE);

    GoToCursor(PARAM_COL, STEP_DIRECTION_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->A_MOVE_AMPLITUDE);

    GoToCursor(PARAM_COL, TURNING_AIM_ROW);
    //if(Walking::GetInstance()->A_MOVE_AIM_ON == true)
    if(true)
        printf("ON     ");
    else
        printf("OFF    ");

    GoToCursor(PARAM_COL, FOOT_HEIGHT_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->Z_MOVE_AMPLITUDE);

    GoToCursor(PARAM_COL, SWING_RIGHTLEFT_ROW);
    //printf("%.1f    ", Walking::GetInstance()->Y_SWAP_AMPLITUDE);

    GoToCursor(PARAM_COL, SWING_TOPDOWN_ROW);
    //printf("%d    ", (int)Walking::GetInstance()->Z_SWAP_AMPLITUDE);

    GoToCursor(PARAM_COL, PELVIS_OFFSET_ROW);
    //printf("%.1f    ", Walking::GetInstance()->PELVIS_OFFSET);

    GoToCursor(PARAM_COL, ARM_SWING_GAIN_ROW);
    //printf("%.1f    ", Walking::GetInstance()->ARM_SWING_GAIN);

    GoToCursor(PARAM_COL, BAL_KNEE_GAIN_ROW);
    //printf("%.2f    ", Walking::GetInstance()->BALANCE_KNEE_GAIN);

    GoToCursor(PARAM_COL, BAL_ANKLE_PITCH_GAIN_ROW);
    //printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN);

    GoToCursor(PARAM_COL, BAL_HIP_ROLL_GAIN_ROW);
    //printf("%.2f    ", Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN);

    GoToCursor(PARAM_COL, BAL_ANKLE_ROLL_GAIN_ROW);
    //printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN);

    GoToCursor(PARAM_COL, P_GAIN_ROW);
    //printf("%d    ", Walking::GetInstance()->P_GAIN);

    GoToCursor(PARAM_COL, I_GAIN_ROW);
    //printf("%d    ", Walking::GetInstance()->I_GAIN);

    GoToCursor(PARAM_COL, D_GAIN_ROW);
    //printf("%d    ", Walking::GetInstance()->D_GAIN);

    GoToCursor(old_col, old_row);
}

void ClearCmd()
{
    PrintCmd("");
}

void PrintCmd(const char *message)
{
    int len = strlen(message);
    GoToCursor(0, CMD_ROW);

    printf( "] %s", message);
    for(int i=0; i<(SCREEN_COL - (len + 2)); i++)
        printf(" ");

    GoToCursor(len + 2, CMD_ROW);
}

void IncreaseValue(bool large)
{
    int col;
    int row;
    if(bBeginCommandMode == true)
    {
        col = Old_Col;
        row = Old_Row;
    }
    else
    {
        col = Col;
        row = Row;
    }

    if(col != PARAM_COL)
        return;

    GoToCursor(col, row);

    switch(row)
    {
    case LOCATION_THREAD_ROW:
        LocationManager::GetInstance()->StartLogging();
        //LocationManager::GetInstance()->Start();
        printf("ON    ");
        break;

        //case X_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->X_OFFSET += 10;
        //	else
        //		Walking::GetInstance()->X_OFFSET += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->X_OFFSET);
        //	break;

        //case Y_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_OFFSET += 10;
        //	else
        //		Walking::GetInstance()->Y_OFFSET += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Y_OFFSET);
        //	break;

    case MOVE_VELOCITY_ROW:
        if(large == true)
            giMoveVelocity += 10;
        else
            giMoveVelocity += 1;
        printf("%d    ", giMoveVelocity);
        break;

    case ROTATE_VELOCITY_ROW:
        if(large == true)
            gdRotateVelocity += 1.0;
        else
            gdRotateVelocity += 0.1;
        printf("%.1f    ", gdRotateVelocity);
        break;

        //case PITCH_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->P_OFFSET += 1.0;
        //	else
        //		Walking::GetInstance()->P_OFFSET += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->P_OFFSET);
        //	break;

        //case YAW_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_OFFSET += 1.0;
        //	else
        //		Walking::GetInstance()->Y_OFFSET += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->Y_OFFSET);
        //	break;

        //case HIP_PITCH_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->HIP_PITCH_OFFSET += 1.0;
        //	else
        //		Walking::GetInstance()->HIP_PITCH_OFFSET += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->HIP_PITCH_OFFSET);
        //	break;

        //case AUTO_BALANCE_ROW:
        //	Walking::GetInstance()->BALANCE_ENABLE = true;
        //	printf("ON    ");
        //	break;

        //case PERIOD_TIME_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->PERIOD_TIME += 10;
        //	else
        //		Walking::GetInstance()->PERIOD_TIME += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->PERIOD_TIME);
        //	break;

        //case DSP_RATIO_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->DSP_RATIO += 0.1;
        //	else
        //		Walking::GetInstance()->DSP_RATIO += 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->DSP_RATIO);
        //	break;

        //case STEP_FORWARDBACK_RATIO_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->STEP_FB_RATIO += 0.1;
        //    else
        //        Walking::GetInstance()->STEP_FB_RATIO += 0.01;
        //    printf("%.2f    ", Walking::GetInstance()->STEP_FB_RATIO);
        //    break;

        //case STEP_FORWARDBACK_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->X_MOVE_AMPLITUDE += 10;
        //	else
        //		Walking::GetInstance()->X_MOVE_AMPLITUDE += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->X_MOVE_AMPLITUDE);
        //	break;

        //case STEP_RIGHTLEFT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_MOVE_AMPLITUDE += 10;
        //	else
        //		Walking::GetInstance()->Y_MOVE_AMPLITUDE += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Y_MOVE_AMPLITUDE);
        //	break;

        //case STEP_DIRECTION_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->A_MOVE_AMPLITUDE += 10;
        //	else
        //		Walking::GetInstance()->A_MOVE_AMPLITUDE += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->A_MOVE_AMPLITUDE);
        //	break;

        //case TURNING_AIM_ROW:
        //	Walking::GetInstance()->A_MOVE_AIM_ON = true;
        //	printf("ON   ");
        //	break;

        //case FOOT_HEIGHT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Z_MOVE_AMPLITUDE += 10;
        //	else
        //		Walking::GetInstance()->Z_MOVE_AMPLITUDE += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Z_MOVE_AMPLITUDE);
        //	break;

        //case SWING_RIGHTLEFT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_SWAP_AMPLITUDE += 0.10;
        //	else
        //		Walking::GetInstance()->Y_SWAP_AMPLITUDE += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->Y_SWAP_AMPLITUDE);
        //	break;

        //case SWING_TOPDOWN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Z_SWAP_AMPLITUDE += 10;
        //	else
        //		Walking::GetInstance()->Z_SWAP_AMPLITUDE += 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Z_SWAP_AMPLITUDE);
        //	break;

        //case PELVIS_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->PELVIS_OFFSET += 1.0;
        //	else
        //		Walking::GetInstance()->PELVIS_OFFSET += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->PELVIS_OFFSET);
        //	break;

        //case ARM_SWING_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->ARM_SWING_GAIN += 0.10;
        //	else
        //		Walking::GetInstance()->ARM_SWING_GAIN += 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->ARM_SWING_GAIN);
        //	break;

        //case BAL_KNEE_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_KNEE_GAIN += 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_KNEE_GAIN += 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_KNEE_GAIN);
        //	break;

        //case BAL_ANKLE_PITCH_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN += 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN += 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN);
        //	break;

        //case BAL_HIP_ROLL_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN += 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN += 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN);
        //	break;

        //case BAL_ANKLE_ROLL_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN += 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN += 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN);
        //	break;

        //case P_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->P_GAIN += 10;
        //    else
        //        Walking::GetInstance()->P_GAIN += 1;
        //    printf("%d    ", Walking::GetInstance()->P_GAIN);
        //    break;

        //case I_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->I_GAIN += 10;
        //    else
        //        Walking::GetInstance()->I_GAIN += 1;
        //    printf("%d    ", Walking::GetInstance()->I_GAIN);
        //    break;

        //case D_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->D_GAIN += 10;
        //    else
        //        Walking::GetInstance()->D_GAIN += 1;
        //    printf("%d    ", Walking::GetInstance()->D_GAIN);
        //    break;
    }

    GoToCursor(col, row);
}

void DecreaseValue(bool large)
{
    int col;
    int row;
    if(bBeginCommandMode == true)
    {
        col = Old_Col;
        row = Old_Row;
    }
    else
    {
        col = Col;
        row = Row;
    }

    if(col != PARAM_COL)
        return;

    GoToCursor(col, row);

    switch(row)
    {
    case LOCATION_THREAD_ROW:
        //Walking::GetInstance()->Stop();
        LocationManager::GetInstance()->StopLogging();
        printf("OFF");
        break;

        //case X_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->X_OFFSET -= 10;
        //	else
        //		Walking::GetInstance()->X_OFFSET -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->X_OFFSET);
        //	break;

        //case Y_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_OFFSET -= 10;
        //	else
        //		Walking::GetInstance()->Y_OFFSET -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Y_OFFSET);
        //	break;

    case MOVE_VELOCITY_ROW:
        if(giMoveVelocity > 0) {
            if(large == true)
                giMoveVelocity -= 10;
            else
                giMoveVelocity -= 1;
            printf("%d    ", giMoveVelocity);
        }
        break;

    case ROTATE_VELOCITY_ROW:
        if(gdRotateVelocity > 0) {
            if(large == true)
                gdRotateVelocity -= 1.0;
            else
                gdRotateVelocity -= 0.1;
            printf("%.1f    ", gdRotateVelocity);
        }
        break;

        //case PITCH_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->P_OFFSET -= 1.0;
        //	else
        //		Walking::GetInstance()->P_OFFSET -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->P_OFFSET);
        //	break;

        //case YAW_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_OFFSET -= 1.0;
        //	else
        //		Walking::GetInstance()->Y_OFFSET -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->Y_OFFSET);
        //	break;

        //case HIP_PITCH_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->HIP_PITCH_OFFSET -= 1.0;
        //	else
        //		Walking::GetInstance()->HIP_PITCH_OFFSET -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->HIP_PITCH_OFFSET);
        //	break;

        //case AUTO_BALANCE_ROW:
        //	Walking::GetInstance()->BALANCE_ENABLE = false;
        //	printf("OFF   ");
        //	break;

        //case PERIOD_TIME_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->PERIOD_TIME -= 10;
        //	else
        //		Walking::GetInstance()->PERIOD_TIME -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->PERIOD_TIME);
        //	break;

        //case DSP_RATIO_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->DSP_RATIO -= 0.1;
        //	else
        //		Walking::GetInstance()->DSP_RATIO -= 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->DSP_RATIO);
        //	break;

        //case STEP_FORWARDBACK_RATIO_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->STEP_FB_RATIO -= 0.1;
        //    else
        //        Walking::GetInstance()->STEP_FB_RATIO -= 0.01;
        //    printf("%.2f    ", Walking::GetInstance()->STEP_FB_RATIO);
        //    break;

        //case STEP_FORWARDBACK_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->X_MOVE_AMPLITUDE -= 10;
        //	else
        //		Walking::GetInstance()->X_MOVE_AMPLITUDE -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->X_MOVE_AMPLITUDE);
        //	break;

        //case STEP_RIGHTLEFT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_MOVE_AMPLITUDE -= 10;
        //	else
        //		Walking::GetInstance()->Y_MOVE_AMPLITUDE -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Y_MOVE_AMPLITUDE);
        //	break;

        //case STEP_DIRECTION_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->A_MOVE_AMPLITUDE -= 10;
        //	else
        //		Walking::GetInstance()->A_MOVE_AMPLITUDE -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->A_MOVE_AMPLITUDE);
        //	break;

        //case TURNING_AIM_ROW:
        //	Walking::GetInstance()->A_MOVE_AIM_ON = false;
        //	printf("OFF   ");
        //	break;

        //case FOOT_HEIGHT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Z_MOVE_AMPLITUDE -= 10;
        //	else
        //		Walking::GetInstance()->Z_MOVE_AMPLITUDE -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Z_MOVE_AMPLITUDE);
        //	break;

        //case SWING_RIGHTLEFT_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Y_SWAP_AMPLITUDE -= 1.0;
        //	else
        //		Walking::GetInstance()->Y_SWAP_AMPLITUDE -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->Y_SWAP_AMPLITUDE);
        //	break;

        //case SWING_TOPDOWN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->Z_SWAP_AMPLITUDE -= 10;
        //	else
        //		Walking::GetInstance()->Z_SWAP_AMPLITUDE -= 1;
        //	printf("%d    ", (int)Walking::GetInstance()->Z_SWAP_AMPLITUDE);
        //	break;

        //case PELVIS_OFFSET_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->PELVIS_OFFSET -= 1.0;
        //	else
        //		Walking::GetInstance()->PELVIS_OFFSET -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->PELVIS_OFFSET);
        //	break;

        //case ARM_SWING_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->ARM_SWING_GAIN -= 1.0;
        //	else
        //		Walking::GetInstance()->ARM_SWING_GAIN -= 0.1;
        //	printf("%.1f    ", Walking::GetInstance()->ARM_SWING_GAIN);
        //	break;

        //case BAL_KNEE_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_KNEE_GAIN -= 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_KNEE_GAIN -= 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_KNEE_GAIN);
        //	break;

        //case BAL_ANKLE_PITCH_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN -= 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN -= 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_PITCH_GAIN);
        //	break;

        //case BAL_HIP_ROLL_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN -= 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN -= 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_HIP_ROLL_GAIN);
        //	break;

        //case BAL_ANKLE_ROLL_GAIN_ROW:
        //	if(large == true)
        //		Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN -= 0.1;
        //	else
        //		Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN -= 0.01;
        //	printf("%.2f    ", Walking::GetInstance()->BALANCE_ANKLE_ROLL_GAIN);
        //	break;

        //case P_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->P_GAIN -= 10;
        //    else
        //        Walking::GetInstance()->P_GAIN -= 1;
        //    printf("%d    ", Walking::GetInstance()->P_GAIN);
        //    break;

        //case I_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->I_GAIN -= 10;
        //    else
        //        Walking::GetInstance()->I_GAIN -= 1;
        //    printf("%d    ", Walking::GetInstance()->I_GAIN);
        //    break;

        //case D_GAIN_ROW:
        //    if(large == true)
        //        Walking::GetInstance()->D_GAIN -= 10;
        //    else
        //        Walking::GetInstance()->D_GAIN -= 1;
        //    printf("%d    ", Walking::GetInstance()->D_GAIN);
        //    break;
    }

    GoToCursor(col, row);
}

void BeginCommandMode()
{
    Old_Col = Col;
    Old_Row = Row;
    ClearCmd();
    GoToCursor(CMD_COL, CMD_ROW);
    bBeginCommandMode = true;
}

void EndCommandMode()
{
    GoToCursor(Old_Col, Old_Row);
    bBeginCommandMode = false;
}

void HelpCmd()
{
    system("clear");
    printf("\n");
    printf(" exit: Exits the program\n");
    printf(" re: Refreshes the screen\n");
    printf(" load: load config\n");
    printf(" save: Saves any changes made\n");
    printf(" mon: Monitoring sensor\n");
    printf("\n");
    printf("       Copyright ROBOTIS CO.,LTD.\n");
    printf("\n");
    printf(" Press any key to continue...");
    _getch();

    DrawScreen();
}

void SaveCmd()
{
    bEdited = false;
}

void MonitorCmd()
{
    int col;
    int row;
    int ch;

    if(bBeginCommandMode == true)
    {
        col = Old_Col;
        row = Old_Row;
    }
    else
    {
        col = Col;
        row = Row;
    }

    system("clear");
    printf("\n");
    //      01234567890123456789012345678901234  Total:35x29
    printf("Robot Position            \n"); // 0
    printf("Gyro R/L                  \n"); // 1
    printf("Accel F/B                 \n"); // 2
    printf("Accel R/L                 \n"); // 3
    printf("ESC (quit), SPACE (stop)   \n");
    printf("W (forward), S (backward), A (left), D (right)   \n");
    printf("Q (turn right), E (turn left)  \n");

    set_stdin();
    while(1)
    {
        //value = LocationStatus::x; //MotionStatus::FB_GYRO;
        GoToCursor(PARAM_COL, 1);
        printf("(%f, %f), %f ", LocationStatus::Position.x, LocationStatus::Position.y, LocationStatus::Handle);

        //	//value = MotionStatus::RL_GYRO; //MotionStatus::RL_GYRO;
        //	if(GyroRL_min > value)
        //		GyroRL_min = value;
        //	if(GyroRL_max < value)
        //		GyroRL_max = value;
        //	GoToCursor(PARAM_COL, 2);
        //	printf("%d (%d~%d)   ", value, GyroRL_min, GyroRL_max);

        //	//value = MotionStatus::FB_ACCEL;
        //	GoToCursor(PARAM_COL, 3);
        //	printf("%d (%d~%d)   ", value, AccelFB_min, AccelFB_max);

        //	//value = MotionStatus::RL_ACCEL;
        //	if(AccelRL_min > value)
        //		AccelRL_min = value;
        //	if(AccelRL_max < value)
        //		AccelRL_max = value;
        //	GoToCursor(PARAM_COL, ROLL_OFFSET_ROW);
        //	printf("%d (%d~%d)   ", value, AccelRL_min, AccelRL_max);

        if(kbhit())
        {
            ch = _getch();
            if(ch == 0x1b) // ESC
                break;
            else if(ch == 0x20) // Space
            {
                StrategyStatus::x = 0.0;
                StrategyStatus::y = 0.0;
                StrategyStatus::w = 0.0;
            }
            else if(ch == 'w' || ch == 'W') // forward
            {
                StrategyStatus::x = giMoveVelocity;
                StrategyStatus::y = 0.0;
                StrategyStatus::w = 0.0;
            }
            else if(ch == 's' || ch == 'S') // forward
            {
                StrategyStatus::x = -giMoveVelocity;
                StrategyStatus::y = 0.0;
                StrategyStatus::w = 0.0;
            }
            else if(ch == 'a' || ch == 'A') // forward
            {
                StrategyStatus::x = 0.0;
                StrategyStatus::y = giMoveVelocity;
                StrategyStatus::w = 0.0;
            }
            else if(ch == 'd' || ch == 'D') // forward
            {
                StrategyStatus::x = 0.0;
                StrategyStatus::y = -giMoveVelocity;
                StrategyStatus::w = 0.0;
            }
            else if(ch == 'q' || ch == 'Q') // forward
            {
                StrategyStatus::x = 0.0;
                StrategyStatus::y = 0.0;
                StrategyStatus::w = gdRotateVelocity;
            }
            else if(ch == 'r' || ch == 'R') // forward
            {
                StrategyStatus::x = 0.0;
                StrategyStatus::y = 0.0;
                StrategyStatus::w = -gdRotateVelocity;
            }
        }
        usleep(500);
    }
    reset_stdin();
    GoToCursor(col, row);
    DrawScreen();
}
