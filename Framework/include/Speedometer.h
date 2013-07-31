/*
 *   Speedometer.h
 *
 *   Author: Wu Chih-En
 *
 */
#include "LocationModule.h"
#include "Motors.h"

namespace Robot
{
	class Speedometer : public LocationModule
	{
	private:
		static Speedometer* m_UniqueInstance;
		Speedometer();

        long mPastMotorPulse[3];

	public:
		static Speedometer* GetInstance() { return m_UniqueInstance; }
		~Speedometer();

        void Process();
        void Initialize(); 
	};
} 
