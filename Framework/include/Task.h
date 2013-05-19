/*
 *   Task.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _Task_H_
#define _Task_H_

#include <string.h>

#include "minIni.h"
#include "StrategyModule.h"

#define TASK_SECTION    "Task"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class Task : public StrategyModule
	{
	private:
		static Task* m_UniqueInstance;
		
		Task();

	public:
		static Task* GetInstance() { return m_UniqueInstance; }
		
		~Task();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
