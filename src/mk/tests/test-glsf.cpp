#include "yocto/utest/run.hpp"
#include "yocto/math/fit/glsf.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

static inline void save(const string &fn, const array<double> &x, const array<double> &y)
{
    ios::ocstream fp(fn,false);
    for(size_t i=1;i<=x.size();++i)
    {
        fp("%g %g\n", x[i], y[i]);
    }
}

namespace
{
    struct diffusion
    {

        double compute( double t, const array<double> &a )
        {
            const double t0    = a[1];
            const double slope = a[2];
            return sqrt( slope*(t-t0) );
        }
        
    };
}



YOCTO_UNIT_TEST_IMPL(glsf)
{
    const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};

    const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};

    const lw_array<double> t1( (double*)_t1, sizeof(_t1)/sizeof(_t1[0]) );
    const lw_array<double> x1( (double*)_x1, sizeof(_x1)/sizeof(_x1[0]) );
    const lw_array<double> t2( (double*)_t2, sizeof(_t2)/sizeof(_t2[0]) );
    const lw_array<double> x2( (double*)_x2, sizeof(_x2)/sizeof(_x2[0]) );

    save("d1.dat",t1,x1);
    save("d2.dat",t2,x2);


    vector<double> z1(t1.size(),0);
    vector<double> z2(t2.size(),0);

    GLS<double>::Samples samples(2);
    GLS<double>::Sample &S1 = samples.append(t1, x1, z1);
    GLS<double>::Sample &S2 = samples.append(t2, x2, z2);

    samples.prepare(3,2);

    S1.link(1,2);
    S1.link(3,1);
    std::cerr << "S1.Gamma=" << S1.Gamma << std::endl;

    S2.link(2,2);
    S2.link(3,1);
    std::cerr << "S2.Gamma=" << S2.Gamma << std::endl;


    diffusion diff;
    GLS<double>::Function F( &diff, & diffusion::compute );

    vector<double> aorg(3);
    vector<bool>   used(3,true);
    double &slope1  = aorg[1];
    double &slope2  = aorg[2];
    double &t0      = aorg[3];
    t0     = -20;
    slope1 = 0.02;
    slope2 = 0.01;

    samples.computeD2(F,aorg,used);
    samples.compute_cinv(0.0);

    //used[3] = false;
    //samples.computeD2(F,aorg,used);

    samples.computeD2_(F,aorg);
    save("f1.dat",t1,z1);
    save("f2.dat",t2,z2);

    std::cerr << "P10_MAX=" << GLS<double>::GET_P10_MAX()<< " / " << GLS<float>::GET_P10_MAX() << std::endl;
    for(int p=samples.p10_min-1;p<=samples.p10_max+1;++p)
    {
        std::cerr << "lambda(" << p << ")=" << samples.compute_lambda(p) << std::endl;
    }

    samples.fit_with(F,aorg, used);
    

}
YOCTO_UNIT_TEST_DONE()
