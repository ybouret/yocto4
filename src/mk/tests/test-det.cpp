#include "yocto/math/kernel/det.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;


template<typename T>
static inline
void perform_det()
{
    std::cerr << std::endl;
    std::cerr << "-- performing " << typeid(T).name() << " -- " << std::endl;
    
    for(size_t n=1;n<=5;++n)
    {
        matrix<T> M(n,n);
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                M[i][j] = T( int(10*(alea<double>()-0.5)) );
            }
        }
        std::cerr << "M" << n << "=" << M << std::endl;
        const T D = determinant_of(M);
        std::cerr << "det(M" << n << ")=" << D << std::endl;
        continue;
        
        matrix<T> A(n,n);
        adjoint(A, M);
        std::cerr << "A" << n << "=" << A << std::endl;
        
      

        if( Fabs(D) > 0 )
        {
            matrix<T> P(n,n);
            tao::mmul(P, A, M);
            std::cerr << "P=" << P << std::endl;
            
#if 0
            vector<T> x(n,numeric<T>::zero);
            vector<T> b(n,numeric<T>::zero);
            for(size_t i=1;i<=n;++i)
            {
                b[i] = T( int(10*(alea<double>()-0.5)) );
            }
            std::cerr << "b=" << b << std::endl;
            tao::mul(x,A,b);
            for(size_t i=1;i<=n;++i) x[i] /= D;
            std::cerr << "x0=" << x << std::endl;
            improve(x, M, A, D, b);
            std::cerr << "x=" << x << std::endl;
#endif
        }
    }
    
}






YOCTO_UNIT_TEST_IMPL(det)
{
    perform_det<double>();
    perform_det<float>();
    perform_det< complex<float> >();
    perform_det<ptrdiff_t>();
}
YOCTO_UNIT_TEST_DONE()
