/*
 *   VisionStatus.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "VisionStatus.h"
#include <stdio.h>
#include <stdlib.h>

using namespace Robot;

tsColor colorBGR (unsigned char B,unsigned char G,unsigned char R)

{

    tsColor color;

    color.ColorEle[BLUE]     = B ;

    color.ColorEle[GREEN]    = G ;

    color.ColorEle[RED]      = R ;



    return color ;

}

//--------------------------------------------------------------------



//--------------------------------------------------------------------

//-----------------------tsBmpPtr-------------------------------------

tsColor tsBmpPtr::GetColor(int x,int y)

{



    if(x<Width && y<Height && x>=0 && y>=0){

        tsColor* ColorPtr;

        ColorPtr = (tsColor*)ImgData;

        return  ColorPtr[x+y*Width];

    }

    else return cBLACK;

}

unsigned char* tsBmpPtr::GetPixelPtr(int x,int y)

{

    if(x<Width && y<Height && x>=0 && y>=0){

        return  &ImgData[Dimension*(x+y*Width)];

    }

    else return NULL;

}

void tsBmpPtr::SetColor(int x,int y ,tsColor cBGR)

{

    if(x<Width && y<Height && x>=0 && y>=0){

        tsColor* ColorPtr;

        ColorPtr = (tsColor*)ImgData;

        ColorPtr[x+y*Width]= cBGR;

    }

}



void tsBmpPtr::SetColor(int x,int y ,unsigned char* Color)

{

    if(x<Width && y<Height && x>=0 && y>=0){

        for( int i=0 ; i<Dimension ; i++ )

            ImgData[Dimension*(x+y*Width)+i] = Color[i];

    }

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------

//--------------------------------------------------------------------

//---------------------tsImgProSet------------------------------------


float tsImgDisModel::Pixel2Distance(int pixel)

{

    if (enable){

        if (ModelNum == NaN) return NaN;

        else{

            if      (pixel > this->DisPixel_Model[ModelNum-2].Pixel) return NaN ;  //this last data is end of data must be zero 

            else if (pixel < this->DisPixel_Model[    0    ].Pixel) return NaN ;

            else{

                int offset = pixel-this->DisPixel_Model[0].Pixel;

                return this->DisPixel_Model[offset].Distance;

            }

        }

    }

    else  return NaN ;

}

//--------------------------------------------------------------------

//--------------------------------------------------------------------
