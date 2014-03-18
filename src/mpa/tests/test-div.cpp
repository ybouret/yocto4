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
        mpn x = 1+_rand.full<uint16_t>();
        if(u.size())
            x *= u.back();
        u.push_back(x);
    }
 
    const size_t n = u.size();
    for(size_t i=1;i<=n;++i)
    {
        const mpn num = u[i];
        const mpn den = u[(n+1)-i];
        const mpn q   = num/den;
        const mpn r   = num-q*den;
        if(r>num) throw exception("invalid division");
    }
    
    for(size_t i=1;i<=n;++i)
    {
        const mpn x = u[i];
        for(size_t j=1;j<=n;++j)
        {
            const mpn y  = u[j];
            const mpn r1 = x%y;
            if(r1>y) throw exception("r1 error");
            if( x != ( (x/y)*y + r1))
                throw exception("mod1 error");
            
            const mpn r2 = y%x;
            if(r2>x) throw exception("r2 error");
            if( y != ( (y/x)*x + r2))
                throw exception("mod2 error");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
