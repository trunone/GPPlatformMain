/*
 *   LocationManager.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LOCATION_MANGER_H_
#define _LOCATION_MANGER_H_

#include <list>
#include <fstream>
#include <iostream>
#include "Status.h"
#include "LocationModule.h"
#include "minIni.h"

#define OFFSET_SECTION "Offset"
#define INVALID_VALUE   -1024.0

namespace Robot
{
	class LocationManager
	{
	private:
		static LocationManager* m_UniqueInstance;
		std::list<LocationModule*> m_Modules;
		bool m_ProcessEnable;
		bool m_Enabled;

		bool m_IsRunning;
		bool m_IsThreadRunning;
		bool m_IsLogging;

		std::ofstream m_LogFileStream;

        LocationManager();

	protected:

	public:
		bool DEBUG_PRINT;

		~LocationManager();

		static LocationManager* GetInstance() { return m_UniqueInstance; }

		bool Initialize();
		bool Reinitialize();
        void Process();
		void SetEnable(bool enable);
		bool GetEnable()				{ return m_Enabled; }
		void AddModule(LocationModule *module);
		void RemoveModule(LocationModule *module);

		void StartLogging();
		void StopLogging();

        void LoadINISettings(minIni* ini);
        void LoadINISettings(minIni* ini, const std::string &section);
        void SaveINISettings(minIni* ini);
        void SaveINISettings(minIni* ini, const std::string &section);
	};
}

#endif
