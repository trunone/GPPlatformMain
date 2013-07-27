#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Normalrand.h"
#define pi 3.141592654

NormalRand::NormalRand()
{
    RandTable=NULL;
    RandTable_size=0;
}
NormalRand::NormalRand(float sigma,float unit)
{
    srand(time(NULL)+rand());
    RandTable=NULL;
    RandTable_size=0;

    float treble_sigma=3*sigma;
    float i,temp;
    float total_value=0;
    for(i=-treble_sigma;i<treble_sigma;i+=unit){
        total_value=total_value+Gaussians(sigma,0,i);
    }

    temp=total_value/(float)RAND_MAX;

    for(i=-treble_sigma;i<treble_sigma;i+=unit){
        this->RandTable_size=this->RandTable_size+Gaussians(sigma,0,i)/temp;
    }

    this->RandTable=new float[this->RandTable_size];

    int shift=0,j,jtime;

    for(i=-treble_sigma;i<treble_sigma;i+=unit){
        jtime=Gaussians(sigma,0,i)/temp;
        for(j=0;j<jtime;j++){
            this->RandTable[shift]=i;
            shift++;
        }
    }
    for(j=0;j<this->RandTable_size;j++){
        shift=rand()%(this->RandTable_size-j);
        temp=RandTable[j];
        RandTable[j]=RandTable[shift+j];
        RandTable[shift+j]=temp;
    }
}
NormalRand::~NormalRand()
{
    delete[] RandTable;
}
float NormalRand::randn(void)
{
    short Seed = rand();
    if( Seed > this->RandTable_size -1 )
    {
        Seed = (this->RandTable_size -1 )*( rand() / (float)RAND_MAX );
    }
    return this->RandTable[Seed];
}

float NormalRand::Gaussians(float sigma,float mu,float x)
{
    float ans;

    ans=( 1 / (sqrt(2*pi)*sigma) )*exp( -0.5*( pow((x-mu),2)/pow(sigma,2) ) );

    return ans;

}

