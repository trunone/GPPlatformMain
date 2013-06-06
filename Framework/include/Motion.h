/*
 *   Motion.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _MOTION_H_
#define _MOTION_H_

#include <string.h>

#include "minIni.h"
#include "StrategyModule.h"

#define TASK_SECTION    "Motion"

namespace Robot
{
	class Motion : public StrategyModule
	{
	private:
		static Motion* m_UniqueInstance;
		
		Motion();

	public:
		static Motion* GetInstance() { return m_UniqueInstance; }
		
		~Motion();

		void Initialize();
		void Process();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
