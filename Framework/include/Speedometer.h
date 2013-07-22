/*
 *   Speedometer.h
 *
 *   Author: Wu Chih-En
 *
 */
#include "LocationModule.h"
#include "Motors.h"
#include "Wheeled.h"
namespace Robot
{
	class Speedometer : public LocationModule
	{
	private:
		static Speedometer* m_UniqueInstance;

		


	public:
		static Speedometer* GetInstance() { return m_UniqueInstance; }
		Speedometer();
		~Speedometer();

	int GetPositionIs(short ,long*);
	void Process();
	void Initialize(); 
	};
} 
