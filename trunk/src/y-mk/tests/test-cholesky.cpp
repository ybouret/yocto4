#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/cholesky.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline 
void perform()
{
	for( size_t n=1; n <= 8; ++n )
	{
		matrix<T> M(n,n);
		M.ld1();
		for( size_t i=1; i <= n; ++i )
		{
			for( size_t j=1; j <= i; ++j )
			{
				if( i != j )
				{
					M[i][j] = M[j][i] = T(0.5) - alea<T>();
				}
				else
				{
					M[i][i] += alea<T>();
				}
			}
			std::cerr << "M=" << M <<  std::endl;
		}
		vector<T> d(n,0);
		if(cholesky<T>::apply( M, d ))
		{
			std::cerr << "d=" << d << std::endl;
			vector<T> b(n,0);
			vector<T> x(n,0);
			for( size_t i=1; i <= n; ++i ) b[i] = T(0.5) - alea<T>();
			std::cerr << "b=" << b << std::endl;
			cholesky<T>::solve( M,d,b,x);
			std::cerr << "x=" << x << std::endl;
		}
		else {
			std::cerr << "// FAILURE" << std::endl;
		}
		
	}
}

YOCTO_UNIT_TEST_IMPL(cholesky)
{
	perform<float>();
	perform<double>();
}
YOCTO_UNIT_TEST_DONE()
