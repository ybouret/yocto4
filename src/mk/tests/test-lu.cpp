#include "yocto/utest/run.hpp"

#include "yocto/mpa/rational.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/rand.hpp"

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
                if(i==j)
                {
                    M[i][j] = (n*n);
                }
                else
                {
                    const int64_t x = -int64_t(n) + int64_t(alea_leq(2*n));
                    //std::cerr << "x=" << x << std::endl;
                    M[i][j] = x;
                }
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

