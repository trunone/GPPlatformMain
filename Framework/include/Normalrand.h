#ifndef NormalrandH
#define NormalrandH

class NormalRand
{
public:
    static NormalRand*  GetInstance() {
        return m_UniqueInstance;
    }

    ~NormalRand();
    float randn(float mean, float sigma);
private:

    NormalRand();
    static NormalRand* m_UniqueInstance;

    float Gaussians(float sigma,float mu,float x);
};
#endif
