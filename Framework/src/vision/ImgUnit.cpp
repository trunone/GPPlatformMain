//---------------------------------------------------------------------------


//#pragma hdrstop

#include "ImgUnit.h"

//#pragma package(smart_init)

/*__fastcall TImgUnit::TImgUnit()
{
}
//---------------------------------------------------------------------------
__fastcall TImgUnit::~TImgUnit()
{
}
//---------------------------------------------------------------------------
*/
unsigned char PixelGrayScale(tsColor Pixel)
{
    return  Pixel.ColorEle[BLUE]*0.114+
            Pixel.ColorEle[GREEN]*0.587+
            Pixel.ColorEle[RED]*0.299;
}
//---------------------------------------------------------------------------
bool PixelBinarization (tsColor Pixel, unsigned char Threshold)
{
    return PixelGrayScale(Pixel) > Threshold;
}
//---------------------------------------------------------------------------
void ImgGrayScale(tsBmpPtr Image)
{
    tsColor* ColorPtr;

    ColorPtr = (tsColor*)Image.ImgData;
    long int i;
    unsigned Threshold;

    for(i=0;i<Image.Height*Image.Width;i++){
        ColorPtr[i] = GrayPixel(PixelGrayScale(ColorPtr[i])) ;
    }
}
//---------------------------------------------------------------------------
void ImgBinarization(tsBmpPtr Image,unsigned char Threshold)
{
    tsColor* ColorPtr;

    ColorPtr = (tsColor*)Image.ImgData;
    long int i;

    for(i=0;i<Image.Height*Image.Width;i++){
        ColorPtr[i] = BinaryPixel( PixelBinarization(ColorPtr[i],Threshold)) ;
    }
}
//---------------------------------------------------------------------------


