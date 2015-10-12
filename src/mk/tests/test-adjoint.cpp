#include "yocto/utest/run.hpp"

#include "yocto/mpa/rational.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/adjoint.hpp"

using namespace yocto;
using namespace math;

template <typename T>
void __test_adjoint()
{

    for(size_t n=1;n<=5;++n)
    {
        std::cerr << std::endl;
        for(size_t iter=0;iter<4;++iter)
        {
            matrix<T> M(n,n);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    const int64_t x = -int64_t(n) + int64_t(alea_leq(2*n));
                    M[i][j] = x;
                }
            }
            std::cerr << "M=" << M << std::endl;
            matrix<T> A(n,n);
            adjoint(A,M);
            std::cerr << "A=" << A << std::endl;
            matrix<T> P(n,n);
            tao::mmul(P,A,M);
            std::cerr << "P=" << P << std::endl;
            const T d = determinant(M);
            std::cerr << "d=" << d << std::endl;
        }
    }
}

template <typename T>
void __test_iadjoint()
{

    for(size_t n=1;n<=5;++n)
    {
        std::cerr << std::endl;
        for(size_t iter=0;iter<4;++iter)
        {
            matrix<T> M(n,n);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    const int64_t x = -int64_t(n) + int64_t(alea_leq(2*n));
                    M[i][j] = x;
                }
            }
            std::cerr << "M=" << M << std::endl;
            matrix<T> A(n,n);
            iadjoint(A,M);
            std::cerr << "A=" << A << std::endl;
            matrix<T> P(n,n);
            tao::mmul(P,A,M);
            std::cerr << "P=" << P << std::endl;
            const T d = ideterminant(M);
            std::cerr << "d=" << d << std::endl;
        }
    }
}





YOCTO_UNIT_TEST_IMPL(adjoint)
{
    __test_adjoint<float>();
    __test_adjoint<double>();
    __test_adjoint< complex<float> >();
    __test_adjoint< complex<double> >();
    __test_adjoint<mpq>();

    __test_iadjoint<int>();

}
YOCTO_UNIT_TEST_DONE()
