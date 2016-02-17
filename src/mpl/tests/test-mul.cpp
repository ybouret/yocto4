#include "yocto/mpl/natural.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/timings.hpp"
#include "yocto/sort/quick.hpp"

using namespace yocto;
using namespace mpl;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl

YOCTO_UNIT_TEST_IMPL(mul)
{
#if 0
    mpn x = mpn::rand(100);
    mpn y = mpn::rand(40);
    mpn z = x*y;
    
    return 0;
#endif
    
    const size_t bits_min    = 256;
    const size_t bits_max    = 1024;
    const size_t bits_del    = bits_max - bits_min + 1;
    const size_t num_per_bit = 4;
    const size_t count       = bits_del * num_per_bit;
    vector<mpn>  v(count,as_capacity);

    std::cerr << "-- Generating " << count << " numbers..." << std::endl;



    v.free();
    for(size_t i=bits_min;i<=bits_max;++i)
    {
        for(size_t j=num_per_bit;j>0;--j)
        {
            const mpn x = mpn::rand(i);
            v.push_back(x);
        }
    }
    c_shuffle(v(),v.size());
    assert(v.size()==count);
    mpn s = 0;

    double time_length = 5;
    std::cerr << "-- Starting MUL for " << time_length << " seconds" << std::endl;

    timings tmx;
    YOCTO_TIMINGS(tmx, time_length,
                  s.ldz();
                  for(size_t i=1,j=count;i<=count;++i,--j)
                  {
                      s = v[i] * v[j];
                  }
                  );
    tmx.speed *= count;
    tmx.speed *= 1e-6;
    std::cerr << "speed=" << tmx.speed << " Mops" << std::endl;

}
YOCTO_UNIT_TEST_DONE()
