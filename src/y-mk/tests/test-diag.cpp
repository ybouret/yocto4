#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/diag.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;


template <typename T>
static inline
void test_diag()
{
    std::cerr << "Testing for <" << typeid(T).name() << ">" << std::endl;
    
    for(size_t iter=1; iter <= 10; ++iter )
    {
        const size_t   n = 1 + alea_lt(10);
        matrix<T> a(n,n);
        
        for(size_t i=1; i<=n; ++i )
        {
            for(size_t j=1; j<=n;++j)
            {
                a[i][j] = (0.5-alea<double>());// + (i==j?a.rows:0);
            }
        }
        std::cerr << "a=" << a << std::endl;
        diag<T>::HessenbergBalance(a);
        std::cerr << "b=" << a << std::endl;
        diag<T>::HessenbergReduce(a);
        std::cerr << "c=" << a << std::endl;
        vector<T> wr(n,0);
        vector<T> wi(n,0);
        diag<T>::HessenbergQR(a, wr,wi);
        std::cerr << "wr=" << wr << std::endl;
        std::cerr << "wi=" << wi << std::endl;
    }
    
}

YOCTO_UNIT_TEST_IMPL(diag)
{
    test_diag<double>();
    test_diag<float>();

}
YOCTO_UNIT_TEST_DONE()
