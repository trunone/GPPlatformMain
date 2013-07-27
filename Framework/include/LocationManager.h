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
#include "LocationStatus.h"
#include "LocationModule.h"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

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
		bool FlagEvaluatuonEnable;		
        	bool FlagCoerceEvaluatuon;
        	TCoordinate Position;
       		double Handle;
		std::ofstream m_LogFileStream;

        qrk::Urg_driver *urg;

        LocationManager();

	protected:

	public:
		bool DEBUG_PRINT;

		~LocationManager();

		static LocationManager* GetInstance() { return m_UniqueInstance; }

		bool Initialize(qrk::Urg_driver*);
		bool Reinitialize();
        void Process();
		void SetEnable(bool enable);
		bool GetEnable()				{ return m_Enabled; }
		void AddModule(LocationModule *module);
		void RemoveModule(LocationModule *module);

		void StartLogging();
		void StopLogging();
	};
}

#endif
