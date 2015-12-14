#include "yocto/utest/run.hpp"
#include "yocto/math/fit/glsf-spec.hpp"
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

        size_t ncall;
        double compute( double t, const array<double> &a )
        {
            ++ncall;
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

    vector<double> aorg(3);
    vector<bool>   used(aorg.size(),true);
    vector<double> aerr(aorg.size());
    double &slope1  = aorg[1];
    double &slope2  = aorg[2];
    double &t0      = aorg[3];

    S1.link(1,3);
    S1.link(2,1);
    std::cerr << "S1.Gamma=" << S1.Gamma << std::endl;

    S2.link(1,3);
    S2.link(2,2);
    std::cerr << "S2.Gamma=" << S2.Gamma << std::endl;


    diffusion diff;
    diff.ncall = 0;
    GLS<double>::Function F( &diff, & diffusion::compute );



    t0     = -100;
    slope1 = 0.02;
    slope2 = 0.01;

    samples.computeD2(F,aorg,used);

    used[3] = false;


    if( samples.fit_with(F,aorg,used,aerr) )
    {
        std::cerr << "ncall=" << diff.ncall << std::endl;
        GLS<double>::display(std::cerr, aorg, aerr);
        used[3] = true;
        if(samples.fit_with(F,aorg,used,aerr))
        {
            GLS<double>::display(std::cerr, aorg, aerr);
            save("f1.dat",t1,z1);
            save("f2.dat",t2,z2);
            std::cerr << "ncall=" << diff.ncall << std::endl;
        }
    }
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(glsf_poly)
{
    GLS<float>::Function poly = _GLS::Create<float,_GLS::Polynomial>();


    const size_t  N     = 20;
    const float   omega = 1.0 + 0.2f*alea<float>();
    vector<float> X(N);
    vector<float> Y(N);

    const size_t  dmax=4;
    matrix<float> Q(dmax+1,N);
    for(size_t i=1;i<=N;++i)
    {
        const float x = (3.14f*(i-1))/N;
        X[i] = x;
        Y[i] = sin(omega*x) + 0.02f*(0.5f-alea<float>());
    }

    GLS<float>::Samples samples(1);


    for(size_t d=0;d<=dmax;++d)
    {
        const size_t m = d+1;
        samples.free();
        samples.append(X,Y,Q[m]);
        vector<float> aorg(m);
        vector<float> aerr(m);
        vector<bool>  used(m,true);

        samples.prepare(m);

        if(samples.fit_with(poly, aorg, used, aerr))
        {
            GLS<float>::display(std::cerr,aorg, aerr);
        }
        else
        {
            std::cerr << "failure ! with d=" << d << std::endl;
        }

    }

            ios::wcstream fp("fits.dat");
            for(size_t i=1;i<=N;++i)
            {
                fp("%g %g", X[i], Y[i]);
                for(size_t j=1;j<=Q.rows;++j)
                {
                    fp(" %g", Q[j][i]);
                }
                fp("\n");
            }



}
YOCTO_UNIT_TEST_DONE()


