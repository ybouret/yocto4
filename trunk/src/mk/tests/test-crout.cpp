#include "yocto/utest/run.hpp"
#include "../../main/tests/support.hpp"
#include <typeinfo>
#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/crout.hpp"

using namespace yocto;
using namespace math;
using namespace yocto;
using namespace math;

namespace {
    
    template <typename T>
    static inline double __RMS( const array<T> &a, const array<T> &b )
    {
        assert(a.size()==b.size());
        double sum = 0;
        for(size_t i=a.size();i>0;--i)
        {
            const double d = Fabs( a[i] - b[i]);
            sum += d*d;
        }
        return sqrt(sum/a.size());
    }
    
    template <typename T>
    static inline void test_crout()
    {
        std::cerr << "crout<" << typeid(T).name() << ">" << std::endl;
        
        for( size_t n=1; n <= 128; n += 1 + alea_lt(8) )
        {
            const  T  diag(float(n+1));
            matrix<T> m(n,n);
            
            for( size_t i=1; i <= n; ++i )
            {
                for( size_t j=1; j <= n; ++j )
                {
                    const T tmp( gen<T>::get() );
                    m[i][j] = tmp;
                    if( i == j )
                        m[i][j] += diag;
                }
            }
            
            matrix<T> mLU = m;
            
            if( !crout<T>::build(mLU) )
            {
                std::cerr << "matrix " << n << "x" << n << " is not invertible!" << std::endl;
                continue;
            }
            
            //! vector + residual
            vector<T> b(n,numeric<T>::zero);
            vector<T> x(n,numeric<T>::zero);
            vector<T> R(n,numeric<T>::zero);
            
            for( size_t i=1; i <= n; ++i )
            {
                x[i] = b[i] = gen<T>::get();
            }
            
            crout<T>::solve( mLU,x );
            tao::mul(R,m,x);
            
            double rms = __RMS(R,b);
            std::cerr << "rms1 [" << n << "x" << n << "]=" << rms << std::endl;
            
            //! apply to matrix inversion
            matrix<T> mInv(n,n);
            mInv.ld1();
            
            crout<T>::solve(mLU,mInv);
            
            matrix<T> p(n,n);
            
            tao::mmul(p,mInv,m);
            
            rms = 0;
            for(size_t i=1; i <=n; ++i )
            {
                for(size_t j=1;j<=n;++j)
                {
                    const double r = Fabs( p[i][j] - (i==j? numeric<T>::one : numeric<T>::zero) );
                    rms += r*r;
                }
            }
            rms = sqrt(rms/n);
            std::cerr << "rms2 [" << n << "x" << n << "]=" << rms << std::endl;
            
            mInv = m;
            if( crout<T>::inverse(mInv) )
            {
                tao::mmul(p,mInv,m);
                
                rms = 0;
                for(size_t i=1; i <=n; ++i )
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        const double r = Fabs( p[i][j] - (i==j? numeric<T>::one : numeric<T>::zero) );
                        rms += r*r;
                    }
                }
                rms = sqrt(rms/n);
                std::cerr << "rms3 [" << n << "x" << n << "]=" << rms << std::endl;
            }
            else
                throw exception("unexpected singular matrix");
        }
        std::cerr << std::endl;
        
        std::cerr << "pseudo_inv<" << typeid(T).name() << ">" << std::endl;
        
        for( size_t n=1; n <= 32; n += 1 + alea_lt(4) )
        {
            const size_t m = n + (1+alea_lt(n));
            matrix<T>    P(n,m);
            for( size_t i=1; i <= n; ++i )
            {
                for( size_t j=1; j <= m; ++j )
                {
                    const T tmp( gen<T>::get() );
                    P[i][j] = tmp;
                }
            }
            //std::cerr << "P=" << P << std::endl;
            matrix<T> M(m,n);
            if( crout<T>::pseudo_inverse(M,P) )
            {
                //std::cerr << "M=" << M << std::endl;
            }
        }
        
    }
    
    template <typename T>
    static inline
    void test_improve()
    {
        for(size_t n=1;n<=15;n += (1+alea_leq(5)))
        {
            const T diag( float(n+1) );
            matrix<T> A(n,n);
            for( size_t i=1; i <= n; ++i )
            {
                for( size_t j=1; j <= n; ++j )
                {
                    const T tmp( gen<T>::get() );
                    A[i][j] = tmp;
                    if( i == j )
                        A[i][j] += diag;
                }
            }
            matrix<T> ALU = A;
            if( !crout<T>::build(ALU))
                continue;
            
            //! vector + residual
            vector<T> b(n,numeric<T>::zero);
            vector<T> x(n,numeric<T>::zero);
            vector<T> c(n,numeric<T>::zero);
            
            for( size_t i=1; i <= n; ++i )
            {
                x[i] = b[i] = gen<T>::get();
            }
            crout<T>::solve(ALU,x);
            
            tao::mul(c, A, x);
            std::cerr << "A="  << A << std::endl;
            std::cerr << "b="  << b << std::endl;
            std::cerr << "x="  << x << std::endl;
            
            std::cerr << "rms0=" << __RMS(b,c) << std::endl;
            
            
            crout<T>::improve(x,A,ALU,b);
            tao::mul(c, A, x);
            std::cerr << "rms" << 1 << "=" << __RMS(b,c) << std::endl;
            std::cerr << "x=" << x << std::endl;
            
            std::cerr << std::endl;
            
        }
    }
}


YOCTO_UNIT_TEST_IMPL(crout)
{
	test_crout<float>();
	test_crout< complex<float> > ();
	test_crout<double>();
	test_crout< complex<double> >();
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(mprove)
{
    test_improve<double>();
}
YOCTO_UNIT_TEST_DONE()


