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
    tao::mmul(P, A, M);
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
        tao::mul(x,A,b);
        for(size_t i=1;i<=n;++i) x[i] /= D;
        std::cerr << "x0=" << x << std::endl;
        improve(x, M, A, D, b);
        std::cerr << "x=" << x << std::endl;
        
        
    }
}

template<typename T>
static inline
void perform_f_det()
{
    std::cerr << std::endl;
    std::cerr << "-- performing for T=" << typeid(T).name() << " --" << std::endl;
    for(size_t n=3;n<=5;++n)
    {
        matrix<T> M(n,n);
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                M[i][j] = T( int(20*(alea<double>()-0.5)) );
            }
        }
        std::cerr << "M" << n << "=" << M << std::endl;
        matrix<T> Mb(M);
        const T D = kernel::f_determinant_of(M);
        std::cerr << "detM" << n << "=" << D << std::endl;
    }
    
}

static inline
void perform_i_det()
{
    std::cerr << std::endl;
    std::cerr << "-- performing for T=" << typeid(ptrdiff_t).name() << " --" << std::endl;
    for(size_t n=3;n<=5;++n)
    {
        matrix<ptrdiff_t> M(n,n);
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                M[i][j] = ptrdiff_t( int(20*(alea<double>()-0.5)) );
            }
        }
        std::cerr << "M" << n << "=" << M << std::endl;
        matrix<ptrdiff_t> Mb(M);
        const ptrdiff_t D = kernel::i_determinant_of(M);
        std::cerr << "detM" << n << "=" << D << std::endl;
    }
    
}





YOCTO_UNIT_TEST_IMPL(det)
{
#if 0
    perform_det<double>();
    perform_det<float>();
    perform_det< complex<float> >();
    perform_det<ptrdiff_t>();
#endif
    
    perform_f_det<float>();
    perform_f_det<double>();
    perform_f_det< complex<float> >();
    perform_f_det< complex<double> >();
    
    perform_i_det();
    
}
YOCTO_UNIT_TEST_DONE()
