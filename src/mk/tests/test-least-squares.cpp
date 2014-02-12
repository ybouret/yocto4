#include "yocto/utest/run.hpp"
#include "yocto/math/fit/least-squares.hpp"
#include "yocto/sequence/lw-array.hpp"

using namespace yocto;
using namespace math;


namespace
{
    struct diffusion
    {
        
        double compute( double t, const array<double> &a )
        {
            const double slope = a[1];
            const double t0    = a[2];
            return sqrt( slope*(t-t0) );
        }
        
    };
}

YOCTO_UNIT_TEST_IMPL(least_squares)
{
    
    
    const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
    
    const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
    
    const lw_array<double> t1( (double*)_t1, sizeof(_t1)/sizeof(_t1[0]) );
    const lw_array<double> x1( (double*)_x1, sizeof(_x1)/sizeof(_x1[0]) );
    const lw_array<double> t2( (double*)_t2, sizeof(_t2)/sizeof(_t2[0]) );
    const lw_array<double> x2( (double*)_x2, sizeof(_x2)/sizeof(_x2[0]) );
    
    vector<double> z1(t1.size(),0);
    vector<double> z2(t2.size(),0);
    
    typedef least_squares<double> LSF;
    typedef LSF::sample           Sample;
    typedef Sample::pointer       SamplePtr;
    diffusion     diff;
    LSF::function F( &diff, & diffusion::compute );
    
    SamplePtr S1( new  Sample(t1,x1,z1) );
    SamplePtr S2( new  Sample(t2,x2,z2) );

    const size_t nvar = 3;
    const size_t npar = 2;
    
    S1->prepare(nvar,npar);
    S2->prepare(nvar,npar);
    {
        S1->Gamma[1][1] = 1;
        S1->Gamma[2][3] = 1;
        std::cerr << "Gamma1=" << S1->Gamma << std::endl;
    }
    
    {
        S2->Gamma[1][2] = 1;
        S2->Gamma[2][3] = 1;
        std::cerr << "Gamma2=" << S2->Gamma << std::endl;
    }
    
    vector<double> a(3,0);
    vector<bool>   used(3,true);
    a[1] = 0.10;
    a[2] = 0.10;
    a[3] = 1;
    
    LSF lsf;
    
    vector<SamplePtr> samples(2,as_capacity);
    samples.push_back(S1);
    samples.push_back(S2);
    
    used[3] = true;
    
    vector<double> aerr( a.size(), 0 );
    
    lsf.verbose = true;
    lsf(F,samples,a,used,aerr);
    
    std::cerr << "aorg=" << a    << std::endl;
    std::cerr << "aerr=" << aerr << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
