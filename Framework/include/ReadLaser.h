/*
 *   ReadLaser.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _READLASER_H_
#define _READLASER_H_

#include <string.h>

#include "minIni.h"
#include "StrategyModule.h"

#define TASK_SECTION    "ReadLaser"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class ReadLaser : public StrategyModule
	{
	private:
		static ReadLaser* m_UniqueInstance;
		
		ReadLaser();

	public:
		static ReadLaser* GetInstance() { return m_UniqueInstance; }
		
		
		//static char hello;
		~ReadLaser();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
