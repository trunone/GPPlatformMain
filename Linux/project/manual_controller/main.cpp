#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "LinuxWheeled.h"
#include "cmd_process.h"


#define PROGRAM_VERSION		"v1.00"
#define MAXNUM_INPUTCHAR	(128)

#define TRUE 1
#define FALSE 0

using namespace Robot;

void sighandler(int sig)
{
    exit(0);
}

int main()
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    char input[MAXNUM_INPUTCHAR];
    char *token;
    int input_len;
    char cmd[80];
    char param[20][30];
    int num_param;
    int iparam[20];

    unsigned int nodeId = 1;

    unsigned int ErrorCode = 0;
    void* keyHandle_;


    printf( "\n[Manual Controller for GPPlatform %s]\n", PROGRAM_VERSION);

    keyHandle_ = VCS_OpenDevice("EPOS2", "MAXON SERIAL V2", "USB", "USB1", &ErrorCode);

    if(keyHandle_ != NULL)
    {
        if(!VCS_SetProtocolStackSettings(keyHandle_, 1000000, 500, &ErrorCode))
        {
            printf( "Open device failure, error code=0x%x\n", ErrorCode);
            exit(0);
        }

        printf( "Sucess!\n");

        while(1)
        {
            gets(input);
            fflush(stdin);
            input_len = strlen(input);
            if(input_len == 0)
                continue;

            token = strtok( input, " " );
            if(token == 0)
                continue;

            strcpy( cmd, token );
            token = strtok( 0, " " );
            num_param = 0;
            while(token != 0)
            {
                strcpy( param[num_param++], token );
                token = strtok( 0, " " );
            }

            if(strcmp(cmd, "exit") == 0)
                break;
            else if(strcmp(cmd, "help") == 0)
                Help();
            else if(strcmp(cmd, "on") == 0)
            {
                unsigned int ErrorCode = 0;
                int IsInFault = FALSE;

                if( VCS_GetFaultState(keyHandle_, nodeId, &IsInFault, &ErrorCode) )
                {
                    if( IsInFault && !VCS_ClearFault(keyHandle_, nodeId, &ErrorCode) )
                    {
                        printf("Clear fault failed!, error code=0x%x\n", ErrorCode);
                        exit(0);
                    }

                    int IsEnabled = FALSE;
                    if( VCS_GetEnableState(keyHandle_, nodeId, &IsEnabled, &ErrorCode) )
                    {
                        if( !IsEnabled && !VCS_SetEnableState(keyHandle_, nodeId, &ErrorCode) )
                        {
                            printf("Set enable state failed!, error code=0x%x\n", ErrorCode);
                        }
                        else
                        {
                        }
                    }
                }
                else
                {
                    printf("Get fault state failed!, error code=0x%x\n", ErrorCode);

                }
            }
            else if(strcmp(cmd, "off") == 0)
            {
                unsigned int ErrorCode = 0;
                int IsInFault = FALSE;

                if( VCS_GetFaultState(keyHandle_, nodeId, &IsInFault, &ErrorCode) )
                {
                    if( IsInFault && !VCS_ClearFault(keyHandle_, nodeId, &ErrorCode) )
                    {
                        printf("Clear fault failed!, error code=0x%x\n", ErrorCode);
                        exit(0);
                    }

                    int IsEnabled = FALSE;
                    if( VCS_GetEnableState(keyHandle_, nodeId, &IsEnabled, &ErrorCode) )
                    {
                        if( IsEnabled && !VCS_SetDisableState(keyHandle_, nodeId, &ErrorCode) )
                        {
                            printf("Set enable state failed!, error code=0x%x\n", ErrorCode);
                        }
                        else
                        {
                        }
                    }
                }
                else
                {
                    printf("Get fault state failed!, error code=0x%x\n");
                }
            }
            else if(strcmp(cmd, "halt") == 0)
            {
                unsigned int errorCode = 0;

                if( !VCS_HaltPositionMovement(keyHandle_, nodeId, &errorCode) )
                {
                    printf("Halt position movement failed!, error code=0x%x\n", errorCode);
                }
            }
            else if(strcmp(cmd, "appm") == 0)
            {
                unsigned int errorCode = 0;

                if( VCS_ActivateProfilePositionMode(keyHandle_, nodeId, &errorCode) )
                {
                    long TargetPosition = 4096;
                    int Absolute = FALSE;
                    int Immediately = TRUE;

                    if( !Absolute )
                    {
                        long PositionIs = 0;

                        if( VCS_GetPositionIs(keyHandle_, nodeId, &PositionIs, &errorCode) )
                            printf("QC: %d\n", PositionIs);
                    }

                    if( !VCS_MoveToPosition(keyHandle_, nodeId, TargetPosition, Absolute, Immediately, &errorCode) )
                    {
                        printf("Move to position failed!, error code=0x%x\n", errorCode);
                    }
                }
                else
                {
                    printf("Activate profile position mode failed!, error code=0x%x\n", errorCode);
                }
            }
            else
                printf(" Bad command! please input 'help'.\n");
        }
    }
    else
    {
        printf("Get fault state failed!, error code=0x%x\n");
        exit(0);
    }

    printf("\nTerminated DXL Manager.\n");
    return 0;
}
