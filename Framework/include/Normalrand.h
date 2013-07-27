#ifndef NormalrandH
#define NormalrandH


class NormalRand{
public:
    NormalRand();
    NormalRand(float sigma,float unit);
    ~NormalRand();

    float randn(void);
private:
    float Gaussians(float sigma,float mu,float x);
    float *RandTable;
    int RandTable_size;
};

#endif
