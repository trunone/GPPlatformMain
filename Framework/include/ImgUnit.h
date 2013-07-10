#ifndef ImgUnitH
#define ImgUnitH

//#include "CommonUnit.h"
//#include "ImgData.h"
#include "VisionStatus.h"


class VisionStatus
{
public:

private:

public:
	// TImgUnit();
	// ~TImgUnit();

    	friend unsigned char PixelGrayScale   (tsColor Pixel);
    	friend bool PixelBinarization(tsColor Pixel, unsigned char Threshold );

    	friend void ImgGrayScale   (tsBmpPtr Image);
    	friend void ImgBinarization(tsBmpPtr Image, unsigned char Threshold);
};



//---------------------------------------------------------------------------
#endif
