/*
 *   Motors.cpp
 *
 *   Author: Wu Chih-En
 *
 */
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "Motors.h"

using namespace Robot;

Motors::Motors()
{    
    mNodeId = 0;
}

Motors::~Motors()
{
	SetDisable();
}

int Motors::OpenDevice(void* motorHandle, int device_id)
{
    unsigned int error_code = 0;
    char device_name[5] = {0,};
    sprintf(device_name, "USB%s", device_id);
    motorHandle = VCS_OpenDevice("EPOS2", "MAXON SERIAL V2", "USB", device_name, &error_code);

	if(motorHandle != NULL)
	{
		if(!VCS_SetProtocolStackSettings(motorHandle, 1000000, 500, &error_code))
		{
			printf( "Open device failure, error code=0x%x\n", error_code);
            return 1;
		}
    }
    else
    {
        printf("Get fault state failed!, error code=0x%x\n", error_code);
        return 1;
    }

}

int Motors::SetEnable(void* motorHandle)
{
    unsigned int error_code = 0;
    if( VCS_GetFaultState(motorHandle, mNodeId, &IsInFault, &error_code) )
    {
        if( IsInFault && !VCS_ClearFault(motorHandle, mNodeId, &error_code) )
        {
            printf("Clear fault failed!, error code=0x%x\n", error_code);
            return 1;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(motorHandle, mNodeId, &IsEnabled, &error_code) )
        {
            if( !IsEnabled && !VCS_SetEnableState(motorHandle, mNodeId, &error_code) )
            {
                printf("Set enable state failed!, error code=0x%x\n", error_code);
                return 1;
            }
        }
     }
     else
     {
            printf("Get fault state failed!, error code=0x%x\n", error_code);
            return 1;
     }
    
}

int Motors::Initialize()
{
}

int Motors::SetDisable(void* motorHandle)
{
    unsigned int error_code = 0;
    int IsInFault = FALSE;

    if( VCS_GetFaultState(motorHandle, mNodeId, &IsInFault, &error_code) )
    {
        if( IsInFault && !VCS_ClearFault(motorHandle, mNodeId, &error_code) )
        {
            printf("Clear fault failed!, error code=0x%x\n", error_code);
            return 1;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(motorHandle, mNodeId, &IsEnabled, &error_code) )
        {
            if( IsEnabled && !VCS_SetDisableState(motorHandle, mNodeId, &error_code) )
            {
                printf("Set enable state failed!, error code=0x%x\n", error_code);
                return 1;
            }
        }
    }
    else
    {
        printf("Get fault state failed!, error code=0x%x\n", error_code);
        return 1;
    }
}
int Motors::ActivateProfileVelocityMode(void* motorHandle)
{
  	unsigned int error_code = 0;
  	if(!VCS_ActivateProfileVelocityMode(motorHandle,mNodeId,&error_code))
  	{
		printf("Activate Profile Velocity Mode failed!, error code=0x%x\n", error_code);
		return 1;
  	}

}
int Motors::SetVelocityProfile(void* motorHandle,int ProfileAcceleration,int ProfileDeceleration)
{
	unsigned int error_code = 0;
	if(!VCS_SetVelocityProfile(motorHandle,mNodeId,ProfileAcceleration,ProfileDeceleration,&error_code))
		{
			printf("Set Velocity Profile failed!, error code=0x%x\n", error_code);
			return 1;
		}
	}	
}
int Motors::SetVelocity(void* motorHandle,long TargetVelocity)
{
	unsigned int error_code = 0;
	while(1)
	{
		if(!VCS_MoveWithVelocity(motorHandle,mNodeId,TargetVelocity,&error_code))
		{
			printf("Move With Velocity failed!, error code=0x%x\n", error_code);
			return 1;
		}
	}
}
int Motors::HaltVelocityMovement(void* motorHandle)
{
	unsigned int error_code = 0;
	if(!VCS_HaltVelocityMovement(motorHandle,mNodeId,&error_code))
	{
		printf("Halt Velocity Movement failed!, error code=0x%x\n", error_code);
		return 1;
	}
}	
int Motors::OpenDeviceAll()
{   
    OpenDevice(motorHandle0, 0);
    OpenDevice(motorHandle1, 1);
    OpenDevice(motorHandle2, 2);
    return 0;
}

int Motors::SetEnableAll()
{
    SetEnable(motorHandle0);
    SetEnable(motorHandle1);
    SetEnable(motorHandle2);
    return 0;
}

int Motors::SetDisableAll()
{
    SetDisable(motorHandle0);
    SetDisable(motorHandle1);
    SetDisable(motorHandle2);
    return 0;
}
