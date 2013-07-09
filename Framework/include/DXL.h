/*
 *  
 *   DXL.h
 *
 */

#include "dynamixel.h"

namespace Robot
{
	class DXL
	{
	private:


	public:
		int deviceIndex = 0;
		int left = 0;
		int right  = 0;		

		int initialize();
		void Degree(int );
		void EndlessTurn(int );
		void dxl_terminate(void);

	};
}
