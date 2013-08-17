/*
 *   Odometer.h
 *
 *   Author: Wu Chih-En
 *
 */
#include "LocationModule.h"
#include "Motors.h"

namespace Robot
{
	class Odometer : public LocationModule
	{
	private:
		static Odometer* m_UniqueInstance;
		Odometer();

        long mPastMotorPulse[3];
        double mWheelDist[3];

        const double mAngle1Sin;
        const double mAngle2Sin;
        const double mAngle3Sin;

        const double mAngle1Cos;
        const double mAngle2Cos;
        const double mAngle3Cos;

        const double mDistConst;

        void GetWheelDistance(short);
        
        double tmpx, tmpy, tmpw;
	public:
		static Odometer* GetInstance() { return m_UniqueInstance; }
		~Odometer();

        void Process();
        void Initialize();
	};
} 
