/*
 *   Motors.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "LinuxEPOS.h"

namespace Robot
{
	class Motors
	{
	private:
	
        int mNodeId;
        static const bool DEBUG_PRINT = false;
		
        int SetEnable(void*);
        int SetDisable(void*);
        int ActivateProfileVelocityMode(void*);
	
        int OpenDevice(void**, char);
	public:
        Motors();
		~Motors();

        void* motorHandle0;
        void* motorHandle1;
        void* motorHandle2;

        int SetEnableAll();
        int SetDisableAll();
        int OpenDeviceAll();
        int ActivateProfileVelocityModeAll();

        int Initialize();

        int SetVelocityProfile(void*, int, int);
        int SetVelocity(void*, long);
        int HaltVelocityMovement(void*);
    };
}

#endif
