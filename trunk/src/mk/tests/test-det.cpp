#include "yocto/math/kernel/det.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/sequence/vector.hpp"

#include <typeinfo>

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
        
        matrix<T> A(n,n);
        adjoint(A, M);
        std::cerr << "A" << n << "=" << A << std::endl;
        
        matrix<T> P(n,n);
        tao::mmul(P, A, M);
        std::cerr << "P=" << P << std::endl;
        
        if( Fabs(D) > 0 )
        {
            
            vector<T> x(n,numeric<T>::zero);
            vector<T> b(n,numeric<T>::zero);
            for(size_t i=1;i<=n;++i)
            {
                b[i] = T( int(20*(alea<double>()-0.5)) );
            }
            std::cerr << "b=" << b << std::endl;
            tao::mul(x,A,b);
            for(size_t i=1;i<=n;++i) x[i] /= D;
            std::cerr << "x0=" << x << std::endl;
            improve(x, M, A, D, b);
            std::cerr << "x=" << x << std::endl;
        }
    }
    
}

static inline
void perform_idet()
{
    std::cerr << std::endl << "-- performing int" << std::endl;
    for(size_t n=1;n<=9;++n)
    {
        for(size_t iter=0;iter<1000;++iter)
        {
            matrix<ptrdiff_t> M(n,n);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    M[i][j] = ptrdiff_t( int(10*(alea<double>()-0.5)) );
                }
            }
            std::cerr << "M" << n << "=" << M << std::endl;
            const ptrdiff_t D = determinant_of(M);
            std::cerr << "det(M" << n << ")=" << D << std::endl;
            
            matrix<ptrdiff_t> A(n,n);
            adjoint(A, M);
            std::cerr << "A" << n << "=" << A << std::endl;
            
            matrix<ptrdiff_t> P(n,n);
            tao::mmul(P, A, M);
            std::cerr << "P=" << P << std::endl;
            
            if(D)
            {
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        if(i==j)
                        {
                            if(P[i][i]!=D)
                                throw exception("Diagonal error");
                        }
                        else
                        {
                            if(P[i][j])
                                throw exception("Extra Diag Error");
                        }
                    }
                }
            }
        }
    }
}







YOCTO_UNIT_TEST_IMPL(det)
{
    perform_det<double>();
    perform_det<float>();
    perform_det< complex<float> >();
    
    perform_idet();
    
}
YOCTO_UNIT_TEST_DONE()
