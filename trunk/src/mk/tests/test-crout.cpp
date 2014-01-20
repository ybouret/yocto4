#include "yocto/utest/run.hpp"
#include "../../main/tests/support.hpp"
#include <typeinfo>
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/crout.hpp"

using namespace yocto;
using namespace math;
using namespace yocto;
using namespace math;

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
		crout<T>  LU(n);
        
		if( !LU.build(mLU) )
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
		algebra<T>::mul(R,m,x);
        
		double rms = 0;
		for(size_t i=1; i <=n; ++i )
		{
			const double r = Fabs( R[i] - b[i] );
			rms += r*r;
		}
		rms = sqrt(rms/n);
		std::cerr << "rms1 [" << n << "x" << n << "]=" << rms << std::endl;
        
        //! apply to memory inversion
        matrix<T> mInv(n,n);
        mInv.ld1();
        
        crout<T>::solve(mLU,mInv);
        
        matrix<T> p(n,n);
        
        algebra<T>::mul(p,mInv,m);
        
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
        if( LU.inverse(mInv) )
        {
            algebra<T>::mul(p,mInv,m);
            
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
        crout<T>  LU(n);
        matrix<T> M(m,n);
        if( LU.pseudo_inverse(M,P) )
        {
            //std::cerr << "M=" << M << std::endl;
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
