#include "yocto/mpl/natural.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace mpl;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl

YOCTO_UNIT_TEST_IMPL(mpn)
{
    std::cerr << "-- Basic Tests" << std::endl;
    {
        mpn n0; _SHOW(n0);
        mpn n1 = 0x12;     _SHOW(n1);
        mpn n2 = 0x0234;   _SHOW(n2);
        mpn n3 = 0x123456; _SHOW(n3);

        for(size_t i=0;i<1000;++i)
        {
            word_t x = _rand.full<word_t>();
            mpn    X = x;
            if(X.lsw()!=x) throw exception("invalid lsw!");
            mpn    Y(X);
            if(Y.lsw()!=x) throw exception("invalid lsw level-2");
            if(X!=Y)       throw exception("invalid copy, level-1");
            if(!(X==Y))    throw exception("invlaid copy, level-2");
            Y.ldz();
            Y = X;
            if(X!=Y)       throw exception("invalid assign");
        }
    }

    std::cerr << "-- Addition Tests" << std::endl;
    {
        vector<mpn> values;
        for(size_t i=0;i<1000;++i)
        {
            word_t x = _rand.full<uint32_t>();
            word_t y = _rand.full<uint32_t>();
            word_t z = x + y;

            mpn X = x;
            mpn Y = y;
            mpn Z = X+Y;

            if(Z!=z) throw exception("invalid addition");

            const mpn tmp = _rand.full<word_t>();
            values.push_back(tmp);
        }
        mpn s1 = 0;
        for(size_t i=values.size();i>0;--i)
        {
            s1 += values[i];
        }
        c_shuffle(values(),values.size());
        mpn s2 = 0;
        for(size_t i=values.size();i>0;--i)
        {
            s2 += values[i];
        }
        std::cerr << "s1=" << s1 << ", s2=" << s2 << std::endl;
        if(s1!=s2) throw exception("invalid addition");
    }




}
YOCTO_UNIT_TEST_DONE()
