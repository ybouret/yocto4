#include "yocto/mpl/rational.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/core/determinant.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/tao.hpp"

using namespace yocto;
using namespace mpl;

YOCTO_UNIT_TEST_IMPL(mpq)
{
    mpq r = 2;
    std::cerr << "r=" << r << std::endl;
    mpq x(1,2);
    mpq y(1,3);
    std::cerr << x << "-" << y << "=" << x-y << std::endl;
    matrix<mpq> M(5);
    matrix<double> A(5);
    for(size_t i=1;i<=M.rows;++i)
    {
        for(size_t j=1;j<=M.cols;++j)
        {
            const int tmp = _rand.full<int16_t>();
            M[i][j] = tmp;
            A[i][j] = tmp;
        }
    }
    std::cerr << "M=" << M << std::endl;
    const mpq dd = math::determinant(M);
    std::cerr << "dd=" << dd << "; " << math::determinant(A) << std::endl;
    
    matrix<mpq> M0(M);
    matrix<mpq> IM(M.rows);
    IM.ld1();
    if( ! math::LU<mpq>::build(M) )
    {
        std::cerr << "Cannot LU..." << std::endl;
    }
    else
    {
        math::LU<mpq>::solve(M,IM);
        std::cerr << "IM=" << IM << std::endl;
        matrix<mpq> P(M.rows);
        math::tao::mmul(P, M0, IM);
        std::cerr << "P=" << P << std::endl;
    }


}
YOCTO_UNIT_TEST_DONE()
