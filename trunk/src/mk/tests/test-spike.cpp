#include "yocto/math/alg/spike.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include <cmath>
#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/quick.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(spike)
{
    size_t N = 120;
    vector<double> x(N,0.0);
    vector<double> y(N,0.0);

    for(size_t i=1;i<=N;++i)
    {
        x[i] = 5.0 * ((double)i + 0.2 * ( alea<double>() - 0.5 ))/N;
        y[i] = sin(x[i]) + x[i]*sin(x[i]*x[i]);
    }

    {
        ios::ocstream fp("data.dat",false);
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g\n", x[i], y[i]);
        }
    }

    vector<spike::pointer> spikes;
    spike::detect(spikes,y.begin(),size_t(1),N);
    
    quicksort(spikes,spike::compare_by_value);


    for(size_t k=1;k<=spikes.size();++k)
    {
        const spike &s = *spikes[k];
        const string filename = vformat("spike%u.dat",unsigned(k));
        ios::ocstream fp(filename,false);
        for(size_t i=s.lower;i<=s.upper;++i)
        {
             fp("%g %g\n", x[i], y[i]);
        }
        
    }
    
    
    spike::detect(spikes,y);
    
    

}
YOCTO_UNIT_TEST_DONE()



