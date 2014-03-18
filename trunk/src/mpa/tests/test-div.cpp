#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
using namespace yocto;


YOCTO_UNIT_TEST_IMPL(div)
{
    std::cerr << std::hex;
    vector<mpn> u;
    for(size_t i=0; i <= 10; ++i)
    {
        mpn x = _rand.full<uint16_t>();
        if(u.size())
            x *= u.back();
        u.push_back(x);
    }
    std::cerr << "u=" << u << std::endl;
 
    const size_t n = u.size();
    for(size_t i=1;i<=n;++i)
    {
        const mpn num = u[i];
        const mpn den = u[(n+1)-i];
        const mpn q   = num/den;
        std::cerr << "q=" << q << std::endl;
        const mpn r   = num-q*den;
        if(r>num) throw exception("invalid remainder");
    }
    
}
YOCTO_UNIT_TEST_DONE()
