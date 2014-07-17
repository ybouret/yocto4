#include "yocto/utest/run.hpp"
#include "yocto/math/q64.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using math::q64_t;

static inline q64_t qsum( const array<q64_t> &Q )
{
    q64_t sum = 0;
    for(size_t i=1;i<=Q.size();++i)
    {
        std::cerr << sum << "+" << Q[i] << "=";
        sum += Q[i];
        std::cerr << sum << std::endl;
    }
    return sum;
}

YOCTO_UNIT_TEST_IMPL(q64)
{
    q64_t q;
    std::cerr << "q=" << q << std::endl;
    
    
    q64_t a(5,3);
    q64_t b(7,2);
    q64_t c(-1,6);
    
    
    std::cerr << a << "+" << b << "=" << a+b << std::endl;
    std::cerr << a << "+" << b << "+" << c << "=" << a+b+c << std::endl;

    q = 0;
    std::cerr << "q=" << q << std::endl;
    q += a;
    std::cerr << "q=" << q << std::endl;
    q += b;
    std::cerr << "q=" << q << std::endl;
    q += c;
    std::cerr << "q=" << q << std::endl;
    
    vector<q64_t> Q;
    
    for(size_t i=10+ alea_leq(20); i>0; --i)
    {
        const int      n = int(1000*(alea<double>()-0.5));
        const unsigned d = 1 + unsigned(200*alea<double>());
        const q64_t    q(n,d);
        std::cerr << "n=" << n << " d=" << d << " q=" << q << std::endl;
        Q.push_back(q);
    }
    std::cerr << "Q0=" << Q << std::endl;
    std::cerr << "sum0=" << qsum(Q) << std::endl;
    quicksort(Q);
    std::cerr << "Q1=" << Q << std::endl;
    std::cerr << "sum1=" << qsum(Q) << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()

