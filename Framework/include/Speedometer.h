/*
 *   Speedometer.h
 *
 *   Author: Wu Chih-En
 *
 */
#include "Motors.h"
namespace Robot
{
	class Speedometer : public Motors
	{
	private:
		static Speedometer* m_UniqueInstance;

		


	public:
		static Speedometer* GetInstance() { return m_UniqueInstance; }
		Speedometer();
		~Speedometer();

	void Process();
	double Motor1Distance,Motor2Distance,Motor3Distance;
	double FI;
	double Motor_x;
	double Motor_dy;
	double Motor_sita; 
	};
} 
