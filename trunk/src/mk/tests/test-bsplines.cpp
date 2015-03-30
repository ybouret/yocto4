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
    
    size_t m=7;
    vector<vtx_t>  P(m,as_capacity);
    vector<double> t(m,as_capacity);
    
    t.push_back(0);
    P.push_back(vtx_t(0,0));
    
    for(size_t i=1;i<m;++i)
    {
        const double tt = t.back() + 0.1 + alea<double>();
        const vtx_t  pp( alea<double>()-0.5, alea<double>()-0.5);
        
        t.push_back(tt);
        P.push_back(pp);
    }
    
    const double tmax = t.back();
    std::cerr << "0 -> " << tmax << std::endl;
    
    size_t N = 5;
    for(size_t i=0;i<=N;++i)
    {
        const double tt = (tmax*i)/N;
        Bsplines<double,vtx_t>::compute(tt, t, P, 3);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
