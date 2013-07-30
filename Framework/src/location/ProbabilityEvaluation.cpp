#include "ProbabilityEvaluation.h"
#include <math.h>
#include "LocationStatus.h"
#include "VisionStatus.h"
using namespace Robot;

using namespace Robot;

ProbEvaluation* ProbEvaluation::m_UniqueInstance = new ProbEvaluation();

ProbEvaluation::ProbEvaluation()
{
    VirtualLineMap = NULL;
}
ProbEvaluation::~ProbEvaluation()
{
    delete[] VirtualLineMap->ImgData;
    delete VirtualLineMap;
}

void ProbEvaluation::AssignVirtualMap(VisionStatus::tsBmpPtr *VirtiulMap)
{
    //VirtualLineMap = new VisionStatus::tsBmpPtr;

    //VirtualLineMap->Width  = VirtiulMap->Width;
    //VirtualLineMap->Height = VirtiulMap->Height;
    //VirtualLineMap->Dimension = 1; //
    //VirtualLineMap->ImgData = new unsigned char[VirtualLineMap->Width * VirtualLineMap->Height * VirtualLineMap->Dimension ];
    //unsigned char bColor;
    //int i,j;
    ////unsigned char Threshold = this->Info->ImgInfo->ProcessSetting.BinaryThreshold ;

    //for(i=0 ; i< VirtualLineMap->Height ;i++){
    //    for(j=0 ; j< VirtualLineMap->Width ;j++){
    //        bColor = VisionStatus::PixelBinarization( VirtiulMap->GetColor(j,i) ,  128 );
    //        VirtualLineMap->SetColor(j,i,&bColor);
    //    }
    //}
    //LocationStatus::enable = true;
}
void ProbEvaluation::ScanLinesInfoUpdate()
{
    //be carefull this point point to Source Image
    VisionStatus::tsBmpPtr SampleImg = VisionStatus::Image;

    float HeadAngle = 0;
    float DetectAngle=0;

    int InterR,ExterR;
    TCoordinate Center,detectP;
    unsigned char Threshold ;

    Center.x = VisionStatus::ProcessSetting.CenterX;
    Center.y = VisionStatus::ProcessSetting.CenterY;
    InterR   = VisionStatus::ProcessSetting.InternalRadius;
    ExterR   = VisionStatus::ProcessSetting.ExternalRadius;
    Threshold = VisionStatus::ProcessSetting.BinaryThreshold;

    bool DetColor;

    int i, j;
    for(i=0; i<ScanLinesNun ; i++){

        DetectAngle =  HeadAngle  +  2*M_PI/ScanLinesNun*i;

        detectP = Center + InterR*(aVector(cos(DetectAngle),sin(DetectAngle)));

        DetColor = VisionStatus::PixelBinarization( SampleImg.GetColor(detectP.x,detectP.y) ,  Threshold );

        while(DetColor == ! EdgeColor){

            detectP.x += cos(DetectAngle);
            detectP.y += sin(DetectAngle);
            if((detectP-Center).Length()>ExterR) break;
            DetColor = VisionStatus::PixelBinarization( SampleImg.GetColor(detectP.x,detectP.y) ,  Threshold );
           // SampleImg.SetColor(detectP.x,detectP.y,cRED);   //for test to draw Info.img
        }

        if((detectP-Center).Length()<=ExterR){
            CameraImageScanLinePixels[i] = (detectP-Center).Length();
            //-------------------------------------------------------------------
            if(VisionStatus::DisModel.enable){
                CameraImageScanLineDistance[i] =
                    VisionStatus::DisModel.Pixel2Distance((int)CameraImageScanLinePixels[i]);
            }
            else CameraImageScanLineDistance[i] = NoDet;
            //-------------------------------------------------------------------
        }

        else{
            CameraImageScanLinePixels[i] = NoDet;
            CameraImageScanLineDistance[i] = NoDet;
        }
    }
}

float* ProbEvaluation::ScanLines(int x,int y ,float angle, float starR, float stopR)
{
    float* ScanLines = new float[ScanLinesNun];

    float DetectAngle=0;

    int InterR,ExterR;
    TCoordinate Pos,detectP;
    Pos.x = x;
    Pos.y = y;
    unsigned char DetColor;

    int i, j;

    for(i=0; i<ScanLinesNun ; i++){

        DetectAngle =  angle + Def_ScanStarAngle +  Def_ScanScale*i;

        detectP = Pos + starR*(aVector(cos(DetectAngle),sin(DetectAngle)));

        if(VirtualLineMap->GetPixelPtr(detectP.x,detectP.y) == NULL) break;
        DetColor = *VirtualLineMap->GetPixelPtr(detectP.x,detectP.y);
        
        while(DetColor == !EdgeColor){

            detectP.x += 1.5*cos(DetectAngle);
            detectP.y += 1.5*sin(DetectAngle);

            if((detectP-Pos).Length() > stopR) break;
            if(VirtualLineMap->GetPixelPtr(detectP.x,detectP.y) == NULL) break;
            DetColor = *VirtualLineMap->GetPixelPtr(detectP.x,detectP.y);

            //this->VisualLineMap.SetColor(detectP.x,detectP.y,cRED);   //for test to draw virtualMap.img
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
    float * vPosScanLines; //visual position scanLines
    float stopR,startR;

    //if ( this->Info->ImgInfo->DisModel.enable){
        startR = Def_ScanStarRadius;//this->Info->ImgInfo->DisModel.DisPixel_Model[0].Distance;
        stopR = Def_ScanStopRadius;//this->Info->ImgInfo->DisModel.DisPixel_Model[this->Info->ImgInfo->DisModel.ModelNum-2].Distance ;

        vPosScanLines = this->ScanLines(x,y,angle,startR,stopR);
        double Probability = 1;
        float deviation;

        for(int i=0 ; i<ScanLinesNun ; i++ ){                 //&& vPosScanLines[i] != NoDet
            if(CameraImageScanLineDistance[i] != NoDet && CameraImageScanLineDistance[i]< stopR ){
                if(vPosScanLines[i] != NoDet){
                    deviation = vPosScanLines[i] - CameraImageScanLineDistance[i];
                    Probability *= NormalDistribution(50,deviation);
                    //Probability *= this->NormalDistribution(GetStandandDeviation(vPosScanLines[i]),deviation);    //sigma us 50cm it will become dynamic
                }
                else{
                    Probability = 0;
                    break;
                }
            }
        }
        delete[] vPosScanLines;
        return Probability;
    //}
    //else return 0;
}
double ProbEvaluation::GetStandandDeviation(double ExpectationDis)
{
    double DisTamp = VisionStatus::DisModel.DisPixel_Model[0].Distance;
    int i=0;
    while(ExpectationDis > DisTamp){
        i++;
        if(i>=VisionStatus::DisModel.ModelNum) break;
        DisTamp = VisionStatus::DisModel.DisPixel_Model[i].Distance;

    }
    double disMax,disMin;

    if( (i-PixelSigma) > 0 && (i+PixelSigma) < VisionStatus::DisModel.ModelNum-2 ){
        disMax = VisionStatus::DisModel.DisPixel_Model[i+PixelSigma].Distance;
        disMin = VisionStatus::DisModel.DisPixel_Model[i-PixelSigma].Distance;
    }
    else if( (i-PixelSigma) < 0 ){
        disMax = VisionStatus::DisModel.DisPixel_Model[PixelSigma-1].Distance;
        disMin = VisionStatus::DisModel.DisPixel_Model[0].Distance;
    }
    else if( (i+PixelSigma) > VisionStatus::DisModel.ModelNum-2  ){
        disMax = VisionStatus::DisModel.DisPixel_Model[VisionStatus::DisModel.ModelNum-2].Distance;
        disMin = VisionStatus::DisModel.DisPixel_Model[VisionStatus::DisModel.ModelNum - PixelSigma].Distance;
    }
    if(  (disMax-disMin)/2 <= 30){
        return 30;
   }
   else
        return (disMax-disMin)/2 ;

}

double ProbEvaluation::NormalDistribution(float sigma , float deviation)
{
   return ( 1/sqrt(2*M_PI*sigma*sigma) ) * exp(-0.5*(deviation/sigma)*(deviation/sigma))*100;
}
//---------------------------------------------------------------------------

