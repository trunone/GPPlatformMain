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
	
		Motors();
		~Motors();
	int mNodeId;
	static const bool DEBUG_PRINT = false;


        void* motorHandle0;
        void* motorHandle1;
        void* motorHandle2;
		
	int SetEnable(void*);
	int SetDisable(void*);
	
        int OpenDevice(void*, int);
	int ActivateProfileVelocityMode(void*);
	int SetVelocityProfile(void*,int,int );
	int SetVelocity(void*,long);
	int HaltVelocityMovement(void*);
	public:
        bool SetEnableAll();
        bool SetDisableAll();
        bool OpenDeviceAll();

	void Initialize();
	};
}

#endif
