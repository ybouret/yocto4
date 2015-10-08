#include "yocto/utest/run.hpp"

#include "yocto/mpa/rational.hpp"
#include "yocto/math/core/lu.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline
void __test_lu()
{
    for(size_t n=1;n<2;++n)
    {
        YOCTO_MATRIX<T> M(n);
        LU<T>::build(M);
    }
}

YOCTO_UNIT_TEST_IMPL(lu)
{
    
    __test_lu<float>();
    __test_lu<double>();
    __test_lu< complex<float> >();
    __test_lu< complex<double> >();
    __test_lu<mpq>();

}
YOCTO_UNIT_TEST_DONE()

