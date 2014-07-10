#include "yocto/math/kernel/det.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/algebra.hpp"

using namespace yocto;
using namespace math;


template<typename T>
static inline
void perform_det()
{
    matrix<T> M(4,4);
    for(size_t i=1;i<=M.rows;++i)
    {
        for(size_t j=1;j<=M.cols;++j)
        {
            M[i][j] = T( int(10*(alea<double>()-0.5)) );
        }
    }
    std::cerr << "M=" << M << std::endl;
    std::cerr << "det(M)=" << determinant_of(M) << std::endl;
    
    matrix<T> A(M.rows,M.cols);
    adjoint(A, M);
    std::cerr << "A=" << A << std::endl;
    
    matrix<T> P(M.rows,M.cols);
    algebra<T>::mul(P, A, M);
    std::cerr << "P=" << P << std::endl;
    
}

YOCTO_UNIT_TEST_IMPL(det)
{
    perform_det<double>();
    
}
YOCTO_UNIT_TEST_DONE()
