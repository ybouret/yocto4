#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "../../y-main/tests/support.hpp"
#include <typeinfo>
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/algebra.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void test_lu()
{
	std::cerr << "lu<" << typeid(T).name() << ">" << std::endl;
	
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
		lu<T> LU(n);
        
		if( !LU.build(mLU) )
		{
			std::cerr << "matrix " << n << "x" << n << " is not invertible!" << std::endl;
			continue;
		}
		vector<T> b(n,0);
		vector<T> x(n,0);
        vector<T> R(n,0);
        
		for( size_t i=1; i <= n; ++i )
		{
			x[i] = b[i] = gen<T>::get();
		}
		LU.solve( mLU,x );
		algebra<T>::mul(R,m,x);
        
		double res = 0;
		for(size_t i=1; i <=n; ++i )
		{
			const double r = Fabs( R[i] - b[i] );
			res += r;
		}
		res /= n;
		std::cerr << "res [" << n << "x" << n << "]=" << res << std::endl;
	}
	std::cerr << std::endl;
	
}

YOCTO_UNIT_TEST_IMPL(lu)
{
	test_lu<float>();
	test_lu< complex<float> > ();
	test_lu<double>();
	test_lu< complex<double> >();
}
YOCTO_UNIT_TEST_DONE()
