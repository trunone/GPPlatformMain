#ifndef ProbabilityEvaluationH
#define ProbabilityEvaluationH
#include "LocationStatus.h"
#include "LocationManager.h"
#include "ImgUnit.h"
#define Def_ScanScale 0.15707963267948966192313216916398
#define Def_ScanStarAngle -1.5707963267948966192313216916398
#define Def_ScanStarRadius 5
#define Def_ScanStopRadius 600
#define ScanLinesNun 24
#define NoDet 0
#define PixelSigma 20

class TLocProbEvaluation :
{
private:
    double NormalDistribution(float sigma , float deviation); //sigma is  NormalDistribution's standard deviation

public:
     TLocProbEvaluation();
     ~TLocProbEvaluation();

    tsBmpPtr *VirtualLineMap; //point to systen virtual map;

    float CameraImageScanLinePixels[ScanLinesNun];

    float *CameraImageScanLineDistance;
    void    AssignVirtualMap(tsBmpPtr *VirtiulMap);
    float*  ScanLines(int x,int y ,float angle, float starR, float stopR);  //for virsual map

    double    GetProbability(int x,int y ,float angle);  // get probability of virtual position

    double GetStandandDeviation(double ExpectationDis) ;
    void   ScanLinesInfoUpdate();  //for image


    //run    ScanLinesInfoUpdate()  update image scanlines info
    //and then run n times main()  get all particle's probability

};
//---------------------------------------------------------------------------
#endif
