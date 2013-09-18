#include "yocto/utest/run.hpp"
#include "yocto/associative/multi-map.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/heap.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(multi_map)
{
    vector<size_t> indx;
    for(size_t n=4;n<=100;++n)
    {
        const size_t q = prev_prime(n-1);
        indx.free();
        size_t       hkey   =  alea_leq(0xFFFFFFFF);
        std::cerr << "Build with " << n << "/" << q << " : hkey=" << hkey << std::endl;
        for(size_t i=0;i<n;++i)
        {
            const size_t j = hkey % n;
            indx.push_back(j+1);
            hkey += q;
        }
        assert(indx.size()==n);
        hsort(indx);
        //std::cerr << "indx=" << indx << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            if(indx[i]!=i) throw exception("invalid index for n=%u", unsigned(n));
        }
    }
}
YOCTO_UNIT_TEST_DONE()
