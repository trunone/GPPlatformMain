#include "ProbabilityEvaluation.h"
#include <math.h>
#include "LocationStatus.h"
#include "VisionStatus.h"

using namespace Robot;
using namespace cv;

ProbEvaluation* ProbEvaluation::m_UniqueInstance = new ProbEvaluation();

ProbEvaluation::ProbEvaluation()
{
   
}
ProbEvaluation::~ProbEvaluation()
{

}
void ProbEvaluation::AssignVirtualMap()
{
/*    VirtualLineMap = new VisionStatus::tsBmpPtr;

    VirtualLineMap->Width  = VirtiulMap->Width;
    VirtualLineMap->Height = VirtiulMap->Height;
    VirtualLineMap->Dimension = 1; //
    VirtualLineMap->ImgData = new unsigned char[VirtualLineMap->Width * VirtualLineMap->Height * VirtualLineMap->Dimension ];
    unsigned char bColor;
    int i,j;
    for(i=0 ; i< VirtualLineMap->Height ;i++){
        for(j=0 ; j< VirtualLineMap->Width ;j++){
            bColor = VisionStatus::PixelBinarization( VirtiulMap->GetColor(j,i) ,  128 );
            VirtualLineMap->SetColor(j,i,&bColor);
        }
    }
*/	
	OriginalMap = cvLoadImage("/GPPlatformMain/Data/jpg/2013sksmap.bmp",0);//load visual map(gray) in bmp format
	
	threshold(OriginalMap, VirtualLineMap, 128, 255, CV_THRESH_BINARY);

	LocationStatus::enable = true;
}
float* ProbEvaluation::ScanLines(int x,int y ,float angle, float starR, float stopR)
{
    float* ScanLines = new float[ScanLinesNum];

    float DetectAngle=0;

    TCoordinate Pos,detectP;
    Pos.x = x;
    Pos.y = y;
	int i;
    unsigned char DetColor;
    for(i=0; i<ScanLinesNum ; i++){

        DetectAngle =  angle + Def_ScanStarAngle +  Def_ScanScale*i;

        detectP = Pos + starR*(aVector(cos(DetectAngle),sin(DetectAngle)));

        if(cvGet2D(&VirtualLineMap,detectP.y,detectP.x).val[0] == NULL) break;
        DetColor = cvGet2D(&VirtualLineMap,detectP.y, detectP.x).val[0];
		
		if(cvGetErrStatus() < 0) break;
        
		while(DetColor == !EdgeColor){

            detectP.x += 1.5*cos(DetectAngle);
            detectP.y += 1.5*sin(DetectAngle);

            if((detectP-Pos).Length() > stopR) break;
          	if(cvGet2D(&VirtualLineMap, detectP.y, detectP.x).val[0] == NULL) break;
 			DetColor = cvGet2D(&VirtualLineMap,detectP.y, detectP.x).val[0];
			if(cvGetErrStatus() < 0) break;
		}

        if((detectP-Pos).Length()<= stopR){
            ScanLines[i] = (detectP-Pos).Length();
        }
        else{
            ScanLines[i] = NoDet;
        }
    }
    return ScanLines;
}

double ProbEvaluation::GetProbability(int x,int y ,float angle)
{
    float* vPosScanLines; //visual position scanLines
    float stopR,startR;
    startR = Def_ScanStarRadius;
    stopR = Def_ScanStopRadius;

    vPosScanLines = ScanLines(x,y,angle,startR,stopR);
    double Probability = 1;
    float deviation;

    for(int i=0 ; i<ScanLinesNum ; i++ ){                 //&& vPosScanLines[i] != NoDet
			if(LocationStatus::data[i]< stopR && LocationStatus::data[i] != 0){
				if(vPosScanLines[i] != NoDet){
					deviation = vPosScanLines[i] - LocationStatus::data[i];
					Probability *= NormalDistribution(50,deviation);
				}
				else{
					Probability = 0;
					break;
				}
			}
	}
    delete[] vPosScanLines;
    return Probability;
}

double ProbEvaluation::NormalDistribution(float sigma , float deviation)
{
   return ( 1/sqrt(2*M_PI*sigma*sigma) ) * exp(-0.5*(deviation/sigma)*(deviation/sigma))*100;
}
