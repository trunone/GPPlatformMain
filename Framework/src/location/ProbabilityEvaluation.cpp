#include "ProbabilityEvaluation.h"
#include <math.h>
#include "LocationStatus.h"
#include "VisionStatus.h"

#define EdgeColor 255

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
	OriginalMap = cvLoadImage("../../../Data/2013sksmap.bmp",0);//load visual map(gray) in bmp format
	
	threshold(OriginalMap, VirtualLineMap, 254, 255, CV_THRESH_BINARY);

	LocationStatus::enable = true;
}

float* ProbEvaluation::ScanLines(int x,int y ,float angle, float starR, float stopR)
{
    float* ScanLines = new float[ScanLinesNum];

    float DetectAngle = 0;

    TCoordinate Pos((double)x, VirtualLineMap.rows - (double)y);
    TCoordinate detectP;
#ifdef DRAW_PARTICLE_SCANLINE
    cv::circle(OriginalMap, Point(Pos.x, Pos.y), 3, Scalar(255, 0, 0), 1, 8, 0);
#endif

	int i;
    unsigned char DetColor;

    for(i=0; i<ScanLinesNum ; i++){

        DetectAngle =  angle + Def_ScanStarAngle +  Def_ScanScale*i;
        detectP = Pos + starR*(aVector(cos(DetectAngle),-sin(DetectAngle)));

        if(detectP.y < 0 || detectP.y > VirtualLineMap.rows || detectP.x < 0 || detectP.x > VirtualLineMap.cols)
            continue;
        DetColor = VirtualLineMap.at<uchar>((int)detectP.y, (int)detectP.x);
        
		while(DetColor != EdgeColor){
            detectP.x += cos(DetectAngle);
            detectP.y -= sin(DetectAngle);

            if((detectP-Pos).Length() > stopR) break;

            if(detectP.y < 0 || detectP.y > VirtualLineMap.rows || detectP.x < 0 || detectP.x > VirtualLineMap.cols)
                break;
            DetColor = VirtualLineMap.at<uchar>((int)detectP.y, (int)detectP.x);
        }
        printf("%d, %f, %f\n", DetColor, detectP.x, detectP.y);

        if((detectP-Pos).Length()<= stopR){
            ScanLines[i] = (detectP-Pos).Length();
#ifdef DRAW_PARTICLE_SCANLINE
            cv::line(OriginalMap, Point(detectP.x, detectP.y), Point(Pos.x, Pos.y), Scalar(255, 0, 0), 1, 8, 0);
#endif
        }
        else{
            ScanLines[i] = NoDet;
        }
    }

#ifdef DRAW_PARTICLE_SCANLINE
    char tmp_ch[100] = {0,};
    sprintf(tmp_ch, "./image/Image.%d.%d.%f.jpg", x, y, angle);
    imwrite(tmp_ch, OriginalMap);
#endif

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

    for(int i=0 ; i<ScanLinesNum ; i++ ) {
        double ScanLine = LocationStatus::LaserData[i]/10.0;
			if( ScanLine < stopR && ScanLine != 0){
				if(vPosScanLines[i] != NoDet){
					deviation = vPosScanLines[i] - ScanLine;
                    printf("%f, ", vPosScanLines[i]);
					Probability *= NormalDistribution(50,deviation);
				}
				else{
					Probability = 0;
					break;
				}
			}
	}
    printf("\n\n");
    delete[] vPosScanLines;
    return Probability;
}

double ProbEvaluation::NormalDistribution(float sigma , float deviation)
{
   return ( 1/sqrt(2*M_PI*sigma*sigma) ) * exp(-0.5*(deviation/sigma)*(deviation/sigma))*100;
}
