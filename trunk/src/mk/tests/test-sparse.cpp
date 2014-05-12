#include "yocto/utest/run.hpp"
#include "yocto/math/sparse/matrix.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void __test_sp()
{
    { sp_matrix<T> S0; }
    
    for(size_t n=1;n<=10;++n)
    {
        sp_matrix<T> S(1+alea_leq(n),1+alea_leq(n));
        const size_t n2 = S.rows * S.cols;
        for(size_t k=n2/2;k>0;--k)
        {
            const size_t i = 1 + alea_lt(S.rows);
            const size_t j = 1 + alea_lt(S.cols);
            S(i,j) = T(alea<float>());
        }
        std::cerr << "S" << n << "  = " << S << std::endl;
        S.transpose();
        std::cerr << "S" << n << "' ="  << S << std::endl;
    }
    
    
    
}

YOCTO_UNIT_TEST_IMPL(sparse)
{
    __test_sp<float>();
    __test_sp<double>();
    __test_sp< complex<float>  >();
    __test_sp< complex<double> >();
}
YOCTO_UNIT_TEST_DONE()


template <typename T>
static inline void sp_solve()
{
    
    const size_t n = 5;
    sp_matrix<T> A(n,n);
    for(size_t i=1;i<=n;++i)
    {
        A(i,i) = alea<double>() > 0.5 ? n : -n;
    }
    
}

YOCTO_UNIT_TEST_IMPL(spcj)
{
    
}
YOCTO_UNIT_TEST_DONE()


