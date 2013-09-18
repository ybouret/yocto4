#include "yocto/utest/run.hpp"
#include "yocto/associative/multi-map.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/primes.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(multi_map)
{
    vector<size_t> indx;
    for(size_t n=4;n<=100;++n)
    {
        const size_t q = prev_prime(n-1);
        indx.free();
        size_t       hkey   =  alea_leq(0xFFFFFFFF);
        
        for(size_t i=0;i<n;++i)
        {
            const size_t j = hkey % n;
            indx.push_back(j+1);
            hkey += q;
        }
        assert(indx.size()==n);
        hsort(indx);
        for(size_t i=1;i<=n;++i)
        {
            if(indx[i]!=i) throw exception("invalid index for n=%u", unsigned(n));
        }
    }
    
    multi_map<string,string> thesaurus;
    for( size_t g=3+alea_leq(100); g>0; --g )
    {
        const string key = gen<string>::get();
        for(size_t i=1+alea_leq(200);i>0;--i)
        {
            const string str = gen<string>::get();
            thesaurus.insert(key,str);
        }
    }
}
YOCTO_UNIT_TEST_DONE()
