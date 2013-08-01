#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Normalrand.h"

NormalRand* NormalRand::m_UniqueInstance = new NormalRand();


NormalRand::NormalRand()
{

}
NormalRand::~NormalRand()
{

}

float NormalRand::randn(float mean, float sigma)
{
	float ans = 0.0;
	float rand_val =0.0;
	for(int i =0; i<12; i++){
		
    	rand_val =((float)rand()/(float)RAND_MAX) * 2 * sigma;   //random in 0 ~ 2sigma
		rand_val = rand_val - sigma; //random in -sigma ~ +sigma
		ans = ans + rand_val;
	}

	return mean + 0.5*ans;
}

float NormalRand::Gaussians(float sigma,float mu,float x)
{
    float ans;

    ans=( 1 / (sqrt(2*M_PI)*sigma) )*exp( -0.5*( pow((x-mu),2)/pow(sigma,2) ) );

    return ans;

}

