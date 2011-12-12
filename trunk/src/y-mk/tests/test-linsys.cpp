#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/linsys.hpp"
#include "../../y-main/tests/support.hpp"
#include <typeinfo>
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/algebra.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void test_linsys()
{
	std::cerr << "linsys<" << typeid(T).name() << ">" << std::endl;
	
	for( size_t n=1; n <= 128; n += 1 + alea_less_than(8) )
	{
		const  T  diag(n+1);
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
		
		linsys<T> lss(n);
		if( !lss.LU(mLU) )
		{
			std::cerr << "matrix " << n << "x" << n << " is not invertible!" << std::endl;
			continue;
		}
		vector<T> b(n,0);
		vector<T> x(n,0);
		for( size_t i=1; i <= n; ++i )
		{
			x[i] = b[i] = gen<T>::get();
		}
		lss( mLU, x );
		algebra<T>::mul(lss,m,x);
		//m.mul( lss, x );
		double res = 0;
		for(size_t i=1; i <=n; ++i )
		{
			const double r = Fabs( lss[i] - b[i] );
			res += r;
		}
		res /= n;
		std::cerr << "res [" << n << "x" << n << "]=" << res << std::endl;
	}
	std::cerr << std::endl;
	
}

YOCTO_UNIT_TEST_IMPL(linsys)
{
	test_linsys<float>();
	test_linsys< complex<float> > ();
	test_linsys<double>();
	test_linsys< complex<double> >();
}
YOCTO_UNIT_TEST_DONE()
