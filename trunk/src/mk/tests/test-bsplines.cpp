#include "yocto/math/dat/b-splines.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(bsplines)
{

    typedef v2d<double> vtx_t;

    size_t m=9;
    vector<vtx_t>  P(m,as_capacity);
    vector<double> t(m,as_capacity);

    t.push_back(0);
    P.push_back(vtx_t(0,0));

    for(size_t i=1;i<m;++i)
    {
        const double tt = t.back() + 1;
        const vtx_t  pp( tt, alea<double>());

        t.push_back(tt);
        P.push_back(pp);
    }

    const double tmax = t.back();
    std::cerr << "0 -> " << tmax << std::endl;

    {
        ios::ocstream fp("data.dat",false);
        for(size_t i=1;i<=t.size();++i)
        {
            fp("%g %g %g\n", t[i], P[i].x, P[i].y);
        }
    }



    {
        ios::ocstream fp("bspl.dat",false);
        size_t N = 300;
        for(size_t i=0;i<=N;++i)
        {
            const double tt = (tmax*i)/N;
            const vtx_t v = Bsplines<double,vtx_t>::compute(tt, t, P, 3);
            fp("%g %g %g\n",tt,v.x,v.y);
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
