/*
 *   ReadVision.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _READVISION_H_
#define _READVISION_H_

#include <string.h>

#include "minIni.h"
#include "StrategyModule.h"

#define TASK_SECTION    "ReadVision"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class ReadVision : public StrategyModule
	{
	private:
		static ReadVision* m_UniqueInstance;
		
		ReadVision();

	public:
		static ReadVision* GetInstance() { return m_UniqueInstance; }
		
		
		//static char hello;
		~ReadVision();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
