#include "yocto/utest/run.hpp"
#include "yocto/math/sparse/matrix.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void __test_sp()
{
    { spmatrix<T> S0; }
    
    for(size_t n=1;n<=10;++n)
    {
        spmatrix<T> S(n);
        const size_t n2 = S.size * S.size;
        for(size_t k=n2/2;k>0;--k)
        {
            const size_t i = 1 + alea_lt(S.size);
            const size_t j = 1 + alea_lt(S.size);
            S(i,j) = T(alea<float>());
        }
        std::cerr << "S" << n << " = " << S << std::endl;
    }
}

YOCTO_UNIT_TEST_IMPL(sparse)
{
    __test_sp<float>();
    __test_sp<double>();
    __test_sp< complex<float> >();
    __test_sp< complex<double> >();

}
YOCTO_UNIT_TEST_DONE()


