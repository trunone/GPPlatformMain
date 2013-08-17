#ifndef _SegmentFunction_H_
#define _SegmentFunction_H_
#include "VisionStatus.h"
#include <vector>
using namespace std;

namespace Robot
{
	
	class SegmentFunction
	{

	private:
		SegmentFunction();
		static SegmentFunction* m_UniqueInstance;
	public:
		static SegmentFunction* GetInstance() { return m_UniqueInstance; }
		~SegmentFunction();
		struct SegmentLocation
		{
			int x, y;
		};
		vector <SegmentLocation> LocationList;
		
		void SegmentationInit(int Xvalue, int Yvalue);
		void SegmentationInsert(int Xvalue, int Yvalue);
		


	};
}
#endif
