#include "yocto/math/stat/int-hist.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/list.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ihist)
{
    vector<int> data;
    for(size_t j=10+alea_leq(100);j>0;--j)
    {
        data.push_back( int(alea_leq(100)) - 50 );
    }

    list<int>      bins;
    vector<size_t> H;

    math::i_histogram(bins, H, data);

}
YOCTO_UNIT_TEST_DONE()
