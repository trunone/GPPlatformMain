#ifndef NormalrandH
#define NormalrandH


class NormalRand{
public:
    NormalRand();
    ~NormalRand();
    float randn(float sigma);
private:
    float Gaussians(float sigma,float mu,float x);
};

#endif
