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
		static int Xmax, Xmin, Ymax, Ymin, PointCnt, ;
		void SegmentationInit(int Xvalue, int Yvalue);
		void SegmentationInsert(int Xvalue, int Yvalue);
		


	};
}
