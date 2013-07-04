/*
 *   Task.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _Task_H_
#define _Task_H_

#include <string.h>

#include "StrategyModule.h"

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
	};
}

#endif
