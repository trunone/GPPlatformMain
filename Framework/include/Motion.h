/*
 *   Motion.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _MOTION_H_
#define _MOTION_H_

#include <string.h>

#include "StrategyModule.h"
#include "Wheeled.h"

namespace Robot
{
	class Motion : public StrategyModule
	{
	private:
		static Motion* m_UniqueInstance;

	public:
		static Motion* GetInstance() { return m_UniqueInstance; }
		
		Motion();
		~Motion();

		void Initialize();
		void Process();
	};
}

#endif
