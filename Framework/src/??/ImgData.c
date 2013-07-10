#include "ImgData.h"

#include <stdio.h>

#include <stdlib.h>

#pragma package(smart_init)





//---------------------------------------------------------------------------





//--------------------------------------------------------------------

//--------------------------------------------------------------------

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

void tsImgProSet::LoadSetting()

{

    float *SetValue;

    int var_number=4;		       	        //parameter num

    SetValue=new float[var_number];



    FILE* fptr;			       	            //open file



    fptr=fopen("ImageSetting.ini","r+");



    if(fptr != NULL){                       // if the file is exist



	    char num_st[20];			        //number string



	    int i;

	    for(i=0;i<var_number;i++){

		    while(fgetc(fptr)!='=');        //determine word

		    fscanf(fptr,"%s",num_st);       //catch string after "="

		    SetValue[i]=atof(num_st);		//string to float

	    }



	    fclose(fptr);				        //close file



        CenterX = SetValue[0];

        CenterY = SetValue[1];

        InternalRadius = SetValue[2];

        ExternalRadius = SetValue[3];

    }

    else{                                   // if the file is not exist

        fclose(fptr);                       //close read ptr

        fptr=fopen("ImageSetting.ini","w"); //creat new file



        //write down parameter

        fprintf(fptr,"Image Setting parameter\n");

        fprintf(fptr,"CenterX = %d\n",320);

        fprintf(fptr,"CenterY = %d\n",240);

        fprintf(fptr,"InternalRadius = %d\n",50);

        fprintf(fptr,"ExternalRadius = %d\n",240);



	    fclose(fptr);				//Ãö³¬ÀÉ®×

    }



}

void tsImgProSet::WriteSetting()

{

    FILE* fptr;			       	            //open file



	fptr=fopen("ImageSetting.ini","w");



    fprintf(fptr,"Image Setting parameter\n");

    

    fprintf(fptr,"CenterX = %d\n",this->CenterX);

    fprintf(fptr,"CenterY = %d\n",this->CenterY);

    fprintf(fptr,"InternalRadius = %d\n",this->InternalRadius);

    fprintf(fptr,"ExternalRadius = %d\n",this->ExternalRadius);



	fclose(fptr);				            //close file

}

//--------------------------------------------------------------------

//--------------------------------------------------------------------



//--------------------------------------------------------------------

//-------------------------tsImgDisModel------------------------------

void  tsImgDisModel::LoadModel()

{

    FILE* fptr;			       	            //open file



    fptr=fopen("ImgDisPixModel.ini","r");



    if(fptr != NULL){                       // if the file is exist

        delete[] this->DisPixel_Model;

        char pixel[20],distance[20];			        //number string





        while(fgetc(fptr)!='=');        //determine word

        fscanf(fptr,"%s",pixel);       //catch string after "="

        int var_number =atoi(pixel);

        this->ModelNum = var_number;

        delete[] this->DisPixel_Model;

        this->DisPixel_Model = new tsImgDisModel::tsDisPixel[var_number];



	    int i;

        for(i=0;i<var_number;i++){



		    while(fgetc(fptr)!='=');        //determine word

		    fscanf(fptr,"%s",pixel);       //catch string after "="

            this->DisPixel_Model[i].Pixel =  atof(pixel);



		    while(fgetc(fptr)!='=');        //determine word

		    fscanf(fptr,"%s",distance);       //catch string after "="

            this->DisPixel_Model[i].Distance = atof(distance);

	    }

	    fclose(fptr);				        //close file

        this->enable = true;

    }

}

void  tsImgDisModel::WriteModel()

{

    FILE* fptr;			       	            //open file



	fptr=fopen("ImgDisPixModel.ini","w");



    fprintf(fptr,"DataNum=%d\n",this->ModelNum);



    for(int i=0 ; i<this->ModelNum ; i++){

        fprintf(fptr,"Pixel =%f \tDistance=%f\n",this->DisPixel_Model[i].Pixel,this->DisPixel_Model[i].Distance);

    }

	fclose(fptr);				//Ãö³¬ÀÉ®×

}

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
