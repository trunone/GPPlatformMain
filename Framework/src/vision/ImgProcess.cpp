#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include "ImgProcess.h"
#include "VisionStatus.h"

using namespace std;
using namespace cv;
using namespace Robot;

String face_cascade_name = "../../../Data/jpg/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
CvMat *Vector1_20cm;
CvMat *AvgVector_20cm;
CvMat *EigenValue_Row_20cm;
CvMat *EigenVector_20cm;
CvMat *FFW_20cm;
CvMat *GfW_20cm;
CvMat *GmW_20cm;
CvMat *MW_20cm;
CvMat *FW_20cm;
CvMat *GW_20cm;
CvMat *BW_20cm;
CvMat *SumW1_20cm;
CvMat *SumW2_20cm;
CvMat *SumW3_20cm;
CvMat *SumW4_20cm;
CvMat *SumW5_20cm;
CvMat *SumW6_20cm;
float Dall_20cm[40*40*6];
float GrandfaDW_20cm  [40*40];
float GrandmaDW_20cm  [40*40];
float MotherDW_20cm  [40*40];
float FatherDW_20cm  [40*40];
float GirlDW_20cm  [40*40];
float BoyDW_20cm  [40*40];
CvMat *Vector1_50cm;
CvMat *AvgVector_50cm;
CvMat *EigenValue_Row_50cm;
CvMat *EigenVector_50cm;
CvMat *FFW_50cm;
CvMat *GfW_50cm;
CvMat *GmW_50cm;
CvMat *MW_50cm;
CvMat *FW_50cm;
CvMat *GW_50cm;
CvMat *BW_50cm;
CvMat *SumW1_50cm;
CvMat *SumW2_50cm;
CvMat *SumW3_50cm;
CvMat *SumW4_50cm;
CvMat *SumW5_50cm;
CvMat *SumW6_50cm;
float Dall_50cm[40*40*6];
float GrandfaDW_50cm  [40*40];
float GrandmaDW_50cm  [40*40];
float MotherDW_50cm  [40*40];
float FatherDW_50cm  [40*40];
float GirlDW_50cm  [40*40];
float BoyDW_50cm  [40*40];
void detection_20cm( Mat Image,Mat Face40size )
{
    int Facedata=0;
    std::vector<Rect> faces;
    Mat Image_gray;
    cvtColor( Image,Image_gray, CV_BGR2GRAY );
    equalizeHist( Image_gray, Image_gray );
    face_cascade.detectMultiScale( Image_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100) );
    Mat Image_Face( Size(faces[0].width,faces[0].height) , CV_8UC3 );
    for(int i=faces[0].y; i<faces[0].y + faces[0].height; i++) {
        for(int j=faces[0].x; j<faces[0].x+faces[0].width; j++) {
            Image_Face.data[Facedata] = Image.data[i*Image.cols*3+(j*3)];
            Image_Face.data[Facedata+1] = Image.data[i*Image.cols*3+(j*3)+1];
            Image_Face.data[Facedata+2] = Image.data[i*Image.cols*3+(j*3)+2];
            Facedata = Facedata+3;
        }
    }
    Mat Face_Gray;
    cvtColor( Image_Face, Face_Gray, CV_BGR2GRAY );
    for(int i=0; i<40; i++) {
        for(int j=0; j<40; j++) {
            Face40size.data[i*40*3+j*3] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
            Face40size.data[i*40*3+j*3+1] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
            Face40size.data[i*40*3+j*3+2] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
        }
    }
}
void detection_50cm( Mat Image,Mat Face40size )
{
    int Facedata=0;
    std::vector<Rect> faces;
    Mat Image_gray;
    cvtColor( Image,Image_gray, CV_BGR2GRAY );
    equalizeHist( Image_gray, Image_gray );
    face_cascade.detectMultiScale( Image_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50) );
    Mat Image_Face( Size(faces[0].width,faces[0].height) , CV_8UC3 );
    for(int i=faces[0].y; i<faces[0].y + faces[0].height; i++) {
        for(int j=faces[0].x; j<faces[0].x+faces[0].width; j++) {
            Image_Face.data[Facedata] = Image.data[i*Image.cols*3+(j*3)];
            Image_Face.data[Facedata+1] = Image.data[i*Image.cols*3+(j*3)+1];
            Image_Face.data[Facedata+2] = Image.data[i*Image.cols*3+(j*3)+2];
            Facedata = Facedata+3;
        }
    }
    Mat Face_Gray;
    cvtColor( Image_Face, Face_Gray, CV_BGR2GRAY );
    for(int i=0; i<40; i++) {
        for(int j=0; j<40; j++) {
            Face40size.data[i*40*3+j*3] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
            Face40size.data[i*40*3+j*3+1] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
            Face40size.data[i*40*3+j*3+2] = Face_Gray.data[(i*Image_Face.rows/40)*Image_Face.cols+(j*Image_Face.cols/40)];
        }
    }
}
void Image_Unify( Mat Before , Mat After)
{
    int Iall=0,Iavg=0,I=0;
    for(int i=0; i<40*40; i++)Iall = Iall+Before.data[i*3];
    //Average gray
    Iavg = Iall/(40*40);
    //Unify brightness(128-Iavg)+Igary
    for(int i=0; i<40*40*3; i++) {
        I = (128-Iavg)+Before.data[i];
        if(I<0)I=0;
        if(I>255)I=255;
        After.data[i] = I;
    }
}
void differences_In_Face(Mat Before , Mat After , Mat Favg)
{
    int fd = 0;
    //The Grandfa's Differences In Face
    for(int i=0; i<40; i++) {
        for(int j=0; j<40; j++) {
            fd = Before.data[i*40*3+j*3] - Favg.data[i*40*3+j*3];
            if(fd<0)fd=0;
            After.data[i*40*3+j*3]=fd;
            After.data[i*40*3+j*3+1]=fd;
            After.data[i*40*3+j*3+2]=fd;
        }
    }
}
void ImgProcess::FaceData()
{
    face_cascade.load( face_cascade_name );
    Mat Grandfa_20cm = imread( "../../../Data/jpg/20cm_0/Grandfa_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Grandma_20cm = imread( "../../../Data/jpg/20cm_0/Grandma_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Mother_20cm = imread( "../../../Data/jpg//20cm_0/Mother_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Father_20cm = imread( "../../../Data/jpg/20cm_0/Father_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Girl_20cm = imread( "../../../Data/jpg/20cm_0/Girl_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Boy_20cm = imread( "../../../Data/jpg/20cm_0/Boy_20cm_0.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Grandfa_Face_20cm( Size(40,40) , CV_8UC3 );
    Mat Grandma_Face_20cm( Size(40,40) , CV_8UC3 );
    Mat Mother_Face_20cm( Size(40,40) , CV_8UC3 );
    Mat Father_Face_20cm( Size(40,40) , CV_8UC3 );
    Mat Girl_Face_20cm( Size(40,40) , CV_8UC3 );
    Mat Boy_Face_20cm( Size(40,40) , CV_8UC3 );
    detection_20cm( Grandfa_20cm,Grandfa_Face_20cm );
    detection_20cm( Grandma_20cm,Grandma_Face_20cm );
    detection_20cm( Mother_20cm,Mother_Face_20cm );
    detection_20cm( Father_20cm,Father_Face_20cm );
    detection_20cm( Girl_20cm,Girl_Face_20cm );
    detection_20cm( Boy_20cm,Boy_Face_20cm );
    Image_Unify( Grandfa_Face_20cm , Grandfa_Face_20cm );
    Image_Unify( Grandma_Face_20cm , Grandma_Face_20cm );
    Image_Unify( Mother_Face_20cm , Mother_Face_20cm );
    Image_Unify( Father_Face_20cm , Father_Face_20cm );
    Image_Unify( Girl_Face_20cm , Girl_Face_20cm );
    Image_Unify( Boy_Face_20cm , Boy_Face_20cm );
    Mat Favg_Face_20cm( Size(40,40) , CV_8UC3 );
    for(int i=0; i<40*40*3; i++)
        Favg_Face_20cm.data[i] = ( Grandfa_Face_20cm.data[i]+Grandma_Face_20cm.data[i]+Mother_Face_20cm.data[i]
                                   +Father_Face_20cm.data[i]+Girl_Face_20cm.data[i]+Boy_Face_20cm.data[i] )/6;
    VisionStatus::Favg_Face_20cm = Favg_Face_20cm;
    differences_In_Face(Grandfa_Face_20cm , Grandfa_Face_20cm , Favg_Face_20cm );
    differences_In_Face(Grandma_Face_20cm , Grandma_Face_20cm , Favg_Face_20cm );
    differences_In_Face(Mother_Face_20cm , Mother_Face_20cm , Favg_Face_20cm );
    differences_In_Face(Father_Face_20cm , Father_Face_20cm , Favg_Face_20cm );
    differences_In_Face(Girl_Face_20cm , Girl_Face_20cm , Favg_Face_20cm );
    differences_In_Face(Boy_Face_20cm , Boy_Face_20cm , Favg_Face_20cm );
    for (int i=0; i<40*40; i++) {
        Dall_20cm[i*6+0]=Grandfa_Face_20cm.data[i*3];
        Dall_20cm[i*6+1]=Grandma_Face_20cm.data[i*3];
        Dall_20cm[i*6+2]=Mother_Face_20cm.data[i*3];
        Dall_20cm[i*6+3]=Father_Face_20cm.data[i*3];
        Dall_20cm[i*6+4]=Girl_Face_20cm.data[i*3];
        Dall_20cm[i*6+5]=Boy_Face_20cm.data[i*3];
    }
    Vector1_20cm=cvCreateMat(40*40,6,CV_32FC1);
    cvSetData(Vector1_20cm,Dall_20cm,Vector1_20cm->step);
    AvgVector_20cm=cvCreateMat(1,6,CV_32FC1);
    EigenValue_Row_20cm=cvCreateMat(6,1,CV_32FC1);
    EigenVector_20cm=cvCreateMat(6,6,CV_32FC1);
    cvCalcPCA(Vector1_20cm,AvgVector_20cm,EigenValue_Row_20cm,EigenVector_20cm,CV_PCA_DATA_AS_ROW);
    int Num_20cm=0;
    float EigenValue_All_20cm=0,EigenValue_Max_20cm = cvGet2D(EigenValue_Row_20cm,0,0).val[0];
    for(int i=1; i<6; i++) {
        EigenValue_All_20cm = cvGet2D(EigenValue_Row_20cm,i,0).val[0];
        if(EigenValue_All_20cm > EigenValue_Max_20cm) {
            EigenValue_Max_20cm = EigenValue_All_20cm;
            Num_20cm=i;
        }
    }
    Mat Feature_Face_20cm( Size(40,40) , CV_8UC3 );
    int Feature_Sum_20cm=0;
    for(int i=0; i<40; i++) {
        for(int j=0; j<40*3; j++) {
            Feature_Sum_20cm = Grandfa_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,0).val[0]
                               +Grandma_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,1).val[0]
                               + Mother_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,2).val[0]
                               + Father_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,3).val[0]
                               + Girl_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,4).val[0]
                               + Boy_Face_20cm.data[i*40*3+j]*cvGet2D(EigenVector_20cm,Num_20cm,5).val[0];
            if (Feature_Sum_20cm<0)Feature_Sum_20cm = 0;
            if (Feature_Sum_20cm>255)Feature_Sum_20cm = 255;
            Feature_Face_20cm.data[i*40*3+j] = Feature_Sum_20cm;
        }
    }
    for(int i=0; i<40*40; i++) {
        VisionStatus::FeatureFaceW_20cm[i]=Feature_Face_20cm.data[i*3];
        /* GrandfaDW_20cm[i]=Grandfa_Face_20cm.data[i*3];
        GrandmaDW_20cm[i]=Grandma_Face_20cm.data[i*3];
               MotherDW_20cm[i]=Mother_Face_20cm.data[i*3];
               FatherDW_20cm[i]=Father_Face_20cm.data[i*3];
               GirlDW_20cm[i]=Girl_Face_20cm.data[i*3];
               BoyDW_20cm[i]=Boy_Face_20cm.data[i*3];*/
    }
    /*    FFW_20cm=cvCreateMat(1,40*40,CV_32FC1);
        cvSetData(FFW_20cm,VisionStatus::FeatureFaceW_20cm,FFW_20cm->step);
        GfW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(GfW_20cm,GrandfaDW_20cm,GfW_20cm->step);
        GmW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(GmW_20cm,GrandmaDW_20cm,GmW_20cm->step);
        MW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(MW_20cm,MotherDW_20cm,MW_20cm->step);
        FW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(FW_20cm,FatherDW_20cm,FW_20cm->step);
        GW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(GW_20cm,GirlDW_20cm,GW_20cm->step);
        BW_20cm=cvCreateMat(40*40,1,CV_32FC1);
        cvSetData(BW_20cm,BoyDW_20cm,BW_20cm->step);
        SumW1_20cm=cvCreateMat(1,1,CV_32FC1);
        SumW2_20cm=cvCreateMat(1,1,CV_32FC1);
        SumW3_20cm=cvCreateMat(1,1,CV_32FC1);
        SumW4_20cm=cvCreateMat(1,1,CV_32FC1);
        SumW5_20cm=cvCreateMat(1,1,CV_32FC1);
        SumW6_20cm=cvCreateMat(1,1,CV_32FC1);
        cvMatMul(FFW_20cm,GfW_20cm,SumW1_20cm);
        cvMatMul(FFW_20cm,GmW_20cm,SumW2_20cm);
        cvMatMul(FFW_20cm,MW_20cm,SumW3_20cm);
        cvMatMul(FFW_20cm,FW_20cm,SumW4_20cm);
        cvMatMul(FFW_20cm,GW_20cm,SumW5_20cm);
        cvMatMul(FFW_20cm,BW_20cm,SumW6_20cm);
        printf("%f\n",cvGet2D(SumW1_20cm,0,0).val[0]);
        printf("%f\n",cvGet2D(SumW2_20cm,0,0).val[0]);
        printf("%f\n",cvGet2D(SumW3_20cm,0,0).val[0]);
        printf("%f\n",cvGet2D(SumW4_20cm,0,0).val[0]);
        printf("%f\n",cvGet2D(SumW5_20cm,0,0).val[0]);
        printf("%f\n",cvGet2D(SumW6_20cm,0,0).val[0]);*/
///////////////////////////////////////////////////////////////////////////////////////////
    Mat Grandfa_50cm = imread( "../../../Data/jpg/50cm_0/Grandfa.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Grandma_50cm = imread( "../../../Data/jpg/50cm_0/Grandma.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Mother_50cm = imread( "../../../Data/jpg/50cm_0/Mother.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Father_50cm = imread( "../../../Data/jpg/50cm_0/Father.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Girl_50cm = imread( "../../../Data/jpg/50cm_0/Girl.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Boy_50cm = imread( "../../../Data/jpg/50cm_0/Boy.jpg" , CV_LOAD_IMAGE_COLOR );
    Mat Grandfa_Face_50cm( Size(40,40) , CV_8UC3 );
    Mat Grandma_Face_50cm( Size(40,40) , CV_8UC3 );
    Mat Mother_Face_50cm( Size(40,40) , CV_8UC3 );
    Mat Father_Face_50cm( Size(40,40) , CV_8UC3 );
    Mat Girl_Face_50cm( Size(40,40) , CV_8UC3 );
    Mat Boy_Face_50cm( Size(40,40) , CV_8UC3 );
    detection_50cm( Grandfa_50cm,Grandfa_Face_50cm );
    detection_50cm( Grandma_50cm,Grandma_Face_50cm );
    detection_50cm( Mother_50cm,Mother_Face_50cm );
    detection_50cm( Father_50cm,Father_Face_50cm );
    detection_50cm( Girl_50cm,Girl_Face_50cm );
    detection_50cm( Boy_50cm,Boy_Face_50cm );
    Image_Unify( Grandfa_Face_50cm , Grandfa_Face_50cm );
    Image_Unify( Grandma_Face_50cm , Grandma_Face_50cm );
    Image_Unify( Mother_Face_50cm , Mother_Face_50cm );
    Image_Unify( Father_Face_50cm , Father_Face_50cm );
    Image_Unify( Girl_Face_50cm , Girl_Face_50cm );
    Image_Unify( Boy_Face_50cm , Boy_Face_50cm );
    Mat Favg_Face_50cm( Size(40,40) , CV_8UC3 );
    for(int i=0; i<40*40*3; i++)
        Favg_Face_50cm.data[i] = ( Grandfa_Face_50cm.data[i]+Grandma_Face_50cm.data[i]+Mother_Face_50cm.data[i]
                                   +Father_Face_50cm.data[i]+Girl_Face_50cm.data[i]+Boy_Face_50cm.data[i] )/6;
    VisionStatus::Favg_Face_50cm = Favg_Face_50cm;
    differences_In_Face(Grandfa_Face_50cm , Grandfa_Face_50cm , Favg_Face_50cm );
    differences_In_Face(Grandma_Face_50cm , Grandma_Face_50cm , Favg_Face_50cm );
    differences_In_Face(Mother_Face_50cm , Mother_Face_50cm , Favg_Face_50cm );
    differences_In_Face(Father_Face_50cm , Father_Face_50cm , Favg_Face_50cm );
    differences_In_Face(Girl_Face_50cm , Girl_Face_50cm , Favg_Face_50cm );
    differences_In_Face(Boy_Face_50cm , Boy_Face_50cm , Favg_Face_50cm );
    for (int i=0; i<40*40; i++) {
        Dall_50cm[i*6+0]=Grandfa_Face_50cm.data[i*3];
        Dall_50cm[i*6+1]=Grandma_Face_50cm.data[i*3];
        Dall_50cm[i*6+2]=Mother_Face_50cm.data[i*3];
        Dall_50cm[i*6+3]=Father_Face_50cm.data[i*3];
        Dall_50cm[i*6+4]=Girl_Face_50cm.data[i*3];
        Dall_50cm[i*6+5]=Boy_Face_50cm.data[i*3];
    }
    Vector1_50cm=cvCreateMat(40*40,6,CV_32FC1);
    cvSetData(Vector1_50cm,Dall_50cm,Vector1_50cm->step);
    AvgVector_50cm=cvCreateMat(1,6,CV_32FC1);
    EigenValue_Row_50cm=cvCreateMat(6,1,CV_32FC1);
    EigenVector_50cm=cvCreateMat(6,6,CV_32FC1);
    cvCalcPCA(Vector1_50cm,AvgVector_50cm,EigenValue_Row_50cm,EigenVector_50cm,CV_PCA_DATA_AS_ROW);
    int Num_50cm=0;
    float EigenValue_All_50cm=0,EigenValue_Max_50cm = cvGet2D(EigenValue_Row_50cm,0,0).val[0];
    for(int i=1; i<6; i++) {
        EigenValue_All_50cm = cvGet2D(EigenValue_Row_50cm,i,0).val[0];
        if(EigenValue_All_50cm > EigenValue_Max_50cm) {
            EigenValue_Max_50cm = EigenValue_All_50cm;
            Num_50cm=i;
        }
    }
    Mat Feature_Face_50cm( Size(40,40) , CV_8UC3 );
    int Feature_Sum_50cm=0;
    for(int i=0; i<40; i++) {
        for(int j=0; j<40*3; j++) {
            Feature_Sum_50cm = Grandfa_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,0).val[0]
                               +Grandma_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,1).val[0]
                               + Mother_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,2).val[0]
                               + Father_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,3).val[0]
                               + Girl_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,4).val[0]
                               + Boy_Face_50cm.data[i*40*3+j]*cvGet2D(EigenVector_50cm,Num_50cm,5).val[0];
            if (Feature_Sum_50cm<0)Feature_Sum_50cm = 0;
            if (Feature_Sum_50cm>255)Feature_Sum_50cm = 255;
            Feature_Face_50cm.data[i*40*3+j] = Feature_Sum_50cm;
        }
    }
    for(int i=0; i<40*40; i++) {
        VisionStatus::FeatureFaceW_50cm[i]=Feature_Face_50cm.data[i*3];
        /*  GrandfaDW_50cm[i]=Grandfa_Face_50cm.data[i*3];
        GrandmaDW_50cm[i]=Grandma_Face_50cm.data[i*3];
               MotherDW_50cm[i]=Mother_Face_50cm.data[i*3];
               FatherDW_50cm[i]=Father_Face_50cm.data[i*3];
               GirlDW_50cm[i]=Girl_Face_50cm.data[i*3];
               BoyDW_50cm[i]=Boy_Face_50cm.data[i*3];*/
    }
    /* FFW_50cm=cvCreateMat(1,40*40,CV_32FC1);
     cvSetData(FFW_50cm,VisionStatus::FeatureFaceW_50cm,FFW_50cm->step);
     GfW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(GfW_50cm,GrandfaDW_50cm,GfW_50cm->step);
     GmW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(GmW_50cm,GrandmaDW_50cm,GmW_50cm->step);
     MW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(MW_50cm,MotherDW_50cm,MW_50cm->step);
     FW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(FW_50cm,FatherDW_50cm,FW_50cm->step);
     GW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(GW_50cm,GirlDW_50cm,GW_50cm->step);
     BW_50cm=cvCreateMat(40*40,1,CV_32FC1);
     cvSetData(BW_50cm,BoyDW_50cm,BW_50cm->step);
     SumW1_50cm=cvCreateMat(1,1,CV_32FC1);
     SumW2_50cm=cvCreateMat(1,1,CV_32FC1);
     SumW3_50cm=cvCreateMat(1,1,CV_32FC1);
     SumW4_50cm=cvCreateMat(1,1,CV_32FC1);
     SumW5_50cm=cvCreateMat(1,1,CV_32FC1);
     SumW6_50cm=cvCreateMat(1,1,CV_32FC1);
     cvMatMul(FFW_50cm,GfW_50cm,SumW1_50cm);
     cvMatMul(FFW_50cm,GmW_50cm,SumW2_50cm);
     cvMatMul(FFW_50cm,MW_50cm,SumW3_50cm);
     cvMatMul(FFW_50cm,FW_50cm,SumW4_50cm);
     cvMatMul(FFW_50cm,GW_50cm,SumW5_50cm);
     cvMatMul(FFW_50cm,BW_50cm,SumW6_50cm);
     printf("%f\n",cvGet2D(SumW1_50cm,0,0).val[0]);
     printf("%f\n",cvGet2D(SumW2_50cm,0,0).val[0]);
     printf("%f\n",cvGet2D(SumW3_50cm,0,0).val[0]);
     printf("%f\n",cvGet2D(SumW4_50cm,0,0).val[0]);
     printf("%f\n",cvGet2D(SumW5_50cm,0,0).val[0]);
     printf("%f\n",cvGet2D(SumW6_50cm,0,0).val[0]);*/
}


