#include "yocto/math/kernel/det.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;


template<typename T>
static inline
void perform_det()
{
    const size_t n = 4;
    matrix<T> M(n,n);
    for(size_t i=1;i<=n;++i)
    {
        for(size_t j=1;j<=n;++j)
        {
            M[i][j] = T( int(10*(alea<double>()-0.5)) );
        }
    }
    std::cerr << "M=" << M << std::endl;
    const T D = determinant_of(M);
    std::cerr << "det(M)=" << D << std::endl;
    
    matrix<T> A(n,n);
    adjoint(A, M);
    std::cerr << "A=" << A << std::endl;
    
    matrix<T> P(n,n);
    algebra<T>::mul(P, A, M);
    std::cerr << "P=" << P << std::endl;
    
    if( Fabs(D) > 0 )
    {
        
        vector<T> x(n,numeric<T>::zero);
        vector<T> b(n,numeric<T>::zero);
        for(size_t i=1;i<=n;++i)
        {
            b[i] = T( int(10*(alea<double>()-0.5)) );
        }
        std::cerr << "b=" << b << std::endl;
        algebra<T>::mul(x,A,b);
        for(size_t i=1;i<=n;++i) x[i] /= D;
        std::cerr << "x0=" << x << std::endl;
        improve(x, M, A, D, b);
        std::cerr << "x=" << x << std::endl;

        
    }
}

YOCTO_UNIT_TEST_IMPL(det)
{
    perform_det<double>();
    
}
YOCTO_UNIT_TEST_DONE()
