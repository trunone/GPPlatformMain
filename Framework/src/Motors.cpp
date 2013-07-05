/*
 *   Motors.cpp
 *
 *   Author: Wu Chih-En
 *
 */
#include <stdio.h>
#include <unistd.h>
#include "Motors.h"

#define FALSE 0

using namespace Robot;

Motors::Motors()
{    
}

Motors::~Motors()
{
	SetDisableAll();
}

int Motors::Initialize()
{
    return 0;
}


int Motors::OpenDevice(void** motorHandle, short unsigned int device_id)
{
    error_code = 0;
    char device_name[5] = {0,};
    sprintf(device_name, "USB%d", device_id);
    *motorHandle = VCS_OpenDevice("EPOS2", "MAXON SERIAL V2", "USB", device_name, &error_code);

	if(*motorHandle != NULL)
	{
		if(!VCS_SetProtocolStackSettings(*motorHandle, 1000000, 500, &error_code))
		{
			fprintf(stderr,  "Open device failure, error code=0x%x\n", error_code);
            return 1;
		}
    }
    else
    {
        fprintf(stderr, "(OpenDevice)Get fault state failed!, error code=0x%x\n", error_code);
        return 1;
    }
    return 0;
}

int Motors::SetEnable(void* motorHandle)
{
    error_code = 0;
    int IsInFault = 0;

    if( VCS_GetFaultState(motorHandle, mNodeId, &IsInFault, &error_code) )
    {
        if( IsInFault && !VCS_ClearFault(motorHandle, mNodeId, &error_code) )
        {
            fprintf(stderr, "Clear fault failed!, error code=0x%x\n", error_code);
            return 1;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(motorHandle, mNodeId, &IsEnabled, &error_code) )
        {
            if( !IsEnabled && !VCS_SetEnableState(motorHandle, mNodeId, &error_code) )
            {
                fprintf(stderr, "Set enable state failed!, error code=0x%x\n", error_code);
                return 1;
            }
        }
    }
    else
    {
            fprintf(stderr, "(SetEnable)Get fault state failed!, error code=0x%x\n", error_code);
            return 1;
    }
    return 0;
}

int Motors::SetDisable(void* motorHandle)
{
    error_code = 0;
    int IsInFault = FALSE;

    if( VCS_GetFaultState(motorHandle, mNodeId, &IsInFault, &error_code) )
    {
        if( IsInFault && !VCS_ClearFault(motorHandle, mNodeId, &error_code) )
        {
            fprintf(stderr, "Clear fault failed!, error code=0x%x\n", error_code);
            return 1;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(motorHandle, mNodeId, &IsEnabled, &error_code) )
        {
            if( IsEnabled && !VCS_SetDisableState(motorHandle, mNodeId, &error_code) )
            {
                fprintf(stderr, "Set enable state failed!, error code=0x%x\n", error_code);
                return 1;
            }
        }
    }
    else
    {
        fprintf(stderr, "Get fault state failed!, error code=0x%x\n", error_code);
        return 1;
    }
    return 0;
}

int Motors::ActivateProfileVelocityMode(void* motorHandle)
{
  	error_code = 0;
  	if(!VCS_ActivateProfileVelocityMode(motorHandle,mNodeId,&error_code))
  	{
		fprintf(stderr, "Activate Profile Velocity Mode failed!, error code=0x%x\n", error_code);
		return 1;
  	}
    return 0;
}

int Motors::SetVelocityProfile(void* motorHandle, int ProfileAcceleration, int ProfileDeceleration)
{
	error_code = 0;
	if(!VCS_SetVelocityProfile(motorHandle,mNodeId,ProfileAcceleration,ProfileDeceleration,&error_code))
    {
        fprintf(stderr, "Set Velocity Profile failed!, error code=0x%x\n", error_code);
        return 1;
    }
    return 0;
}

int Motors::SetVelocity(void* motorHandle, long TargetVelocity)
{
	unsigned int error_code = 0;
    if(!VCS_MoveWithVelocity(motorHandle,mNodeId,TargetVelocity,&error_code))
    {
        fprintf(stderr, "Move With Velocity failed!, error code=0x%x\n", error_code);
        return 1;
    }
    return 0;
}

int Motors::HaltVelocityMovement(void* motorHandle)
{
	error_code = 0;
	if(!VCS_HaltVelocityMovement(motorHandle,mNodeId,&error_code))
	{
		fprintf(stderr, "Halt Velocity Movement failed!, error code=0x%x\n", error_code);
		return 1;
	}
    return 0;
}

int Motors::GetIncEncoderParameter(void* motorHandle, unsigned int* pEncoderResolution, int* pInvertedPolarity)
{
    error_code = 0;
    if(!VCS_GetIncEncoderParameter(motorHandle, mNodeId, pEncoderResolution, pInvertedPolarity, &error_code))
    {
        fprintf(stderr, "GetIncEncoderParameter failed!, error code=0x%x\n", error_code);
        return 1;
    }
    return 0;
}

int Motors::OpenDeviceAll()
{
    if(OpenDevice(&motorHandle0, 0) || OpenDevice(&motorHandle1, 1) || OpenDevice(&motorHandle2, 2))
        return 1;
    return 0;
}

int Motors::SetEnableAll()
{
    if(SetEnable(motorHandle0) || SetEnable(motorHandle1) || SetEnable(motorHandle2))
        return 1;
    return 0;
}

int Motors::SetDisableAll()
{
    if( SetDisable(motorHandle0) || SetDisable(motorHandle1) || SetDisable(motorHandle2))
        return 1;
    return 0;
}

int Motors::SetVelocityProfileAll(int ProfileAcceleration, int ProfileDeceleration)
{
    if(SetVelocityProfile(&motorHandle0, ProfileAcceleration, ProfileDeceleration) || SetVelocityProfile(&motorHandle1, ProfileAcceleration, ProfileDeceleration) || SetVelocityProfile(&motorHandle2, ProfileAcceleration, ProfileDeceleration))
        return 1;
    return 0;
}

int Motors::SetVelocityAll(long Velocity1, long Velocity2, long Velocity3)
{
    if(SetVelocity(&motorHandle0, Velocity1) || SetVelocity(&motorHandle1, Velocity2) || SetVelocity(&motorHandle2, Velocity3))
        return 1;
    return 0;
}

int Motors::ActivateProfileVelocityModeAll()
{
    if(ActivateProfileVelocityMode(motorHandle0) || ActivateProfileVelocityMode(motorHandle1) || ActivateProfileVelocityMode(motorHandle2))
        return 1;
    return 0;
}

int Motors::GetIncEncoderParameterAll(
    unsigned int* pEncoderResolution1, int* pInvertedPolarity1,
    unsigned int* pEncoderResolution2, int* pInvertedPolarity2,
    unsigned int* pEncoderResolution3, int* pInvertedPolarity3)
{
    if(GetIncEncoderParameter(&motorHandle0, pEncoderResolution1, pInvertedPolarity1) || GetIncEncoderParameter(&motorHandle1, pEncoderResolution2, pInvertedPolarity2) || GetIncEncoderParameter(&motorHandle2, pEncoderResolution3, pInvertedPolarity3))
        return 1;
    return 0;
}

