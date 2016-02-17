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

YOCTO_UNIT_TEST_IMPL(sub)
{
    const size_t bits_min    = 128;
    const size_t bits_max    = 1024;
    const size_t bits_del    = bits_max - bits_min + 1;
    const size_t num_per_bit = 16;
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
    assert(v.size()==count);


    quicksort(v);
    mpn sum = 0;
    for(size_t i=1;i<=v.size();++i) sum += v[i];

    const double time_length = 5;
    timings tmx;
    std::cerr << "-- Starting SUB for " << time_length << " seconds" << std::endl;

    mpn s;

    YOCTO_TIMINGS(tmx, time_length,
                  s = sum;
                  for(size_t i=count;i>0;--i)
                  {
                      s -= v[i];
                  }
                  );
    tmx.speed *= count;
    tmx.speed *= 1e-6;
    std::cerr << "speed=" << tmx.speed << " Mops" << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
