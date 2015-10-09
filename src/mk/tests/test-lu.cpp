#include "yocto/utest/run.hpp"

#include "yocto/mpa/rational.hpp"
#include "yocto/math/core/lu.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline
void __test_lu()
{
    for(size_t n=1;n<=4;++n)
    {
        YOCTO_MATRIX<T> M(n);
        M.ld1();
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                if(i==j) M[i][j] = (n*n);
            }
        }
        std::cerr << "M=" << M << std::endl;
        if(!LU<T>::build(M))
        {
            std::cerr << "Can't build LU" << std::endl;
        }
        else
        {
            std::cerr << "LU=" << M << std::endl;
        }
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

