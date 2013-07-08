//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#include "ParticleFiliterSimu.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)
//#pragma resource "*.dfm"
//TForm1 *Form1;
//---------------------------------------------------------------------------
 TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    this->info = new TInfo;
    TCommonUnit::info = this->info;


    this->LocProbEvaluation = new TLocProbEvaluation;
    //----------------------------------------------------
    this->Map = new Graphics::TBitmap;
    this->Map->LoadFromFile("Line.bmp");
    this->MapImgPtr = (unsigned char *)this->Map->ScanLine[this->Map->Height-1];
    //----------------------------------------------------
    this->Source = new Graphics::TBitmap;
    this->Source->PixelFormat = pf24bit;
    this->Source->Width = 160*4;
    this->Source->Height = 120*4;
    this->SourceImgPtr=(unsigned char *)this->Source->ScanLine[this->Source->Height-1];
    //----------------------------------------------------
    this->OutPut = new Graphics::TBitmap;
    this->OutPut->PixelFormat = pf24bit;
    this->OutPut->Width = this->Source->Width;
    this->OutPut->Height = this->Source->Height;
    this->OutPutImgPtr = (unsigned char *)this->OutPut->ScanLine[this->Source->Height-1];
    //----------------------------------------------------
    this->SimuOmniDrctImg = new TSimuOmniDrctImg;
    this->SimuOmniImage->AssignInputImg (this->MapImgPtr    , this->Map->Width    , this->Map->Height    , 3);
    this->SimuOmniImage->AssignOutputImg(this->SourceImgPtr , this->Source->Width , this->Source->Height , 3);
    this->SimuOmniImage->CameraPosition.x = this->Map->Width/2;
    this->SimuOmniImage->CameraPosition.y = this->Map->Height/2;
    this->SimuOmniImage->CameraRataAngle = 0.0;
    //----------------------------------------------------
    this->info->ImageInfo.Image.ImgData     = SourceImgPtr;
    this->info->ImageInfo.Image.Height      = this->Source->Height;
    this->info->ImageInfo.Image.Width       = this->Source->Width;
    this->info->ImageInfo.Image.Dimension   = 3;
}

//---------------------------------------------------------------------------
