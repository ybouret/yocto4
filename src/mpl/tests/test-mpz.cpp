#include "yocto/mpl/integer.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"

using namespace yocto;
using namespace mpl;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl

YOCTO_UNIT_TEST_IMPL(mpz)
{

    {
        for(integer_t i=-1000;i<=1000;i+=500)
        {
            mpz z = i;
            _SHOW(z);
        }
    }

    std::cerr << "-- Comparision tests" << std::endl;
    {
        vector<mpz>       Z;
        vector<integer_t> z;
        for(size_t i=0;i<10000;++i)
        {
            const integer_t w = _rand.full<integer_t>();
            const mpz       W = w;
            z.push_back(w);
            Z.push_back(W);
        }
        c_shuffle(z(),z.size());
        c_shuffle(Z(),Z.size());
        quicksort(z);
        quicksort(Z);
        for(size_t i=z.size();i>0;--i)
        {
            if(z[i]!=Z[i]) throw exception("invalid comparison!");
        }
    }

}
YOCTO_UNIT_TEST_DONE()
