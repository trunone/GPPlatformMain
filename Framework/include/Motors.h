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
        static const short unsigned int mNodeId = 1;
        static const bool DEBUG_PRINT = false;

		void* motorHandle0;
        void* motorHandle1;
        void* motorHandle2;

        int OpenDevice(void**, short unsigned int);
        int SetEnable(void*);
        int SetDisable(void*);
        int SetVelocityProfile(void*, int, int);
        int SetVelocity(void*, long);
        int ActivateProfileVelocityMode(void*);
        int HaltVelocityMovement(void*);
        int GetIncEncoderParameter(void*, unsigned int*, int*);

        unsigned int error_code;

	public:
        Motors();
		~Motors();

        int Initialize();

        int SetEnableAll();
        int SetDisableAll();
        int OpenDeviceAll();
        int SetVelocityProfileAll(int, int);
        int SetVelocityAll(long, long, long);
        int ActivateProfileVelocityModeAll();
        int HaltVelocityMoventAll();
        int GetIncEncoderParameterAll(unsigned int*, int*, unsigned int*, int*, unsigned int*, int*);
   };
}

#endif
