#include "yocto/math/stat/int-hist.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/list.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ihist)
{
    vector<int> data;
    for(size_t j=10+alea_leq(200);j>0;--j)
    {
        data.push_back( int(alea_leq(100)) - 50 );
    }

    list<int>      bins;
    vector<size_t> H;

    math::i_histogram(bins, H, data);

    ios::ocstream fp("ihist.dat",false);

    size_t j=1;
    for( list<int>::iterator i=bins.begin();i!=bins.end();++i,++j)
    {
        fp("%d %u\n", *i, unsigned(H[j]));
    }
}
YOCTO_UNIT_TEST_DONE()
