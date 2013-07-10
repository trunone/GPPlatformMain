/*
 *   VisionStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _VISION_STATUS_H_
#define _VISION_STATUS_H_
#define cBLACK      colorBGR(0,0,0)
#define cWHITE      colorBGR(255,255,255)
#define cRED        colorBGR(0,0,255)
#define cGREEN      colorBGR(0,255,0)
#define cBLUE       colorBGR(255,0,0)
#define cYELLOW     colorBGR(0,255,255)
#define cMAGENTA    colorBGR(255,0,255)
#define cCYAN       colorBGR(255,0,255)
#define GrayPixel(Scale)    colorBGR(Scale,Scale,Scale)
#define BinaryPixel(White)  (White)?cWHITE:cBLACK
#define EdgeColor 1
#define NaN 0
#define Def_ImgWidth    640
#define Def_ImgHeight   480
#define Def_ImgByte     3
#define Def_ImgSize     307200  //640*480
#define Def_ImgSizeM3   921600  //640*480*3

namespace Robot
{
	class VisionStatus
	{
	private:

	public:
	typedef enum{BLUE=0 , GREEN=1 , RED=2 , B=0 , G=1 , R=2 }BGRcolor;

	typedef enum{Y=0 , U=1 , V=2}YUVcolor;



	typedef struct{

    		unsigned char ColorEle[3];

	}tsColor;



	typedef struct{

    		unsigned char* ImgData;

    		int Width;

    		int Height;

    		int Dimension;

    		tsColor         GetColor(int x,int y);

    		unsigned char*  GetPixelPtr(int x,int y);

    		void    SetColor(int x,int y ,tsColor cBGR);

    		void    SetColor(int x,int y ,unsigned char* Color);

		}tsBmpPtr;



	typedef struct{



    		int CenterX;

    		int CenterY;

    		int InternalRadius;

    		int ExternalRadius;



    		void LoadSetting();

    		void WriteSetting();



    		unsigned char BinaryThreshold;



	}tsImgProSet;



	typedef struct{

    		bool enable;

    	typedef struct{

        	float Distance;

        	float Pixel;

    	}tsDisPixel;



    	tsDisPixel *DisPixel_Model ;

    	int ModelNum;

    	float Pixel2Distance(int pixel);

    	void  LoadModel();

    	void  WriteModel();



	}tsImgDisModel;



	tsColor colorBGR (unsigned char B ,unsigned char G , unsigned char R);



	//----------------------------------------

    	typedef struct{

            int X;

            int Y;

    	}tsCoordinate;



    	typedef struct{

        	short int Cx;

        	short int Cy;

        	short int Cr;

        	short int PixelDistance;

        	double Angle;

    	}tsObjectiveInfo;



    	typedef struct{

            tsCoordinate LeftUp;

            tsCoordinate RightDown;

            unsigned int countcolor;

    	}tsRecord_Blockinfo;

	//---------------------------------------------------------------------------



	enum{OrangeLabel = 0x01, YellowLabel = 0x02, BlueLabel = 0x04

        	, GreenLabel = 0x08, BlackLabel = 0x10, RedLabel = 0x20

        	, OthersLabel = 0x80};



	struct ColorRange

	{

        	double HueMax;

        	double HueMin;

        	double SaturationMax;

        	double SaturationMin;

        	double BrightnessMax;

        	double BrightnessMin;

	};



	struct CIRCLE

	{

    		short int CCX;

    		short int CCY;

    		short int Rr;

	};

	typedef struct{

    		unsigned char Y;

    		unsigned char U;

    		unsigned char V;



	}tsYUVData;

	//--------------------------------------------------------------------------

	};
}

#endif
