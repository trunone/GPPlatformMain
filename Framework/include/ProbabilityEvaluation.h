#ifndef ProbabilityEvaluationH
#define ProbabilityEvaluationH
#include "LocationStatus.h"
#include "VisionStatus.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#define Def_ScanScale 0.15707963267948966192313216916398
#define Def_ScanStarAngle -1.5707963267948966192313216916398
//#define SsanRange 3.6651914291880921115397506138261   //210

#define Def_ScanStarRadius 5
#define Def_ScanStopRadius 600

#define ScanLinesNum 24
#define NoDet 0
#define PixelSigma 20

namespace Robot{

	class ProbEvaluation
	{
	private:
    	double NormalDistribution(float sigma , float deviation); //sigma is  NormalDistribution's standard deviation
		static ProbEvaluation* m_UniqueInstance;
	public:
    	 ProbEvaluation();
    	~ProbEvaluation();

		static ProbEvaluation* GetInstance() { return m_UniqueInstance; }
        Mat OriginalMap;
		Mat VirtualLineMap; //point to systen virtual map;

    	void AssignVirtualMap();
    	float* ScanLines(int x,int y ,float angle, float starR, float stopR);  //for virsual map
       	double GetProbability(int x,int y ,float angle);  // get probability of virtual position

		double GetStandandDeviation(double ExpectationDis) ;
		void ScanLinesInfoUpdate();  //for image


	    //run    ScanLinesInfoUpdate()  update image scanlines info
	    //and then run n times main()  get all particle's probability

	};
}
#endif
