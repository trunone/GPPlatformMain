/*
 *   StrategyManager.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STRATEGY_MANGER_H_
#define _STRATEGY_MANGER_H_

#include <list>
#include <fstream>
#include <iostream>
#include "StrategyStatus.h"
#include "StrategyModule.h"
#include "minIni.h"

#define OFFSET_SECTION "Offset"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class StrategyManager
	{
	private:
		static StrategyManager* m_UniqueInstance;
		std::list<StrategyModule*> m_Modules;
		bool m_ProcessEnable;
		bool m_Enabled;

		bool m_IsRunning;
		bool m_IsThreadRunning;
		bool m_IsLogging;

		std::ofstream m_LogFileStream;

        StrategyManager();

	protected:

	public:
		bool DEBUG_PRINT;

		~StrategyManager();

		static StrategyManager* GetInstance() { return m_UniqueInstance; }

		bool Initialize();
		bool Reinitialize();
        void Process();
		void SetEnable(bool enable);
		bool GetEnable()				{ return m_Enabled; }
		void AddModule(StrategyModule *module);
		void RemoveModule(StrategyModule *module);

		void StartLogging();
		void StopLogging();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
