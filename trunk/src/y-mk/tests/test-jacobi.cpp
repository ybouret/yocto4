#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/jacobi.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void test_jacobi()
{
	for( size_t n=1; n <= 64; n += 1 + alea_less_than<size_t>(8) )
	{
		matrix<T> m(n,n);
		for( size_t i=1; i <=n; ++i )
		{
			for( size_t j=i; j <= n; ++j )
			{
				m[i][j] = m[j][i] = alea<T>() - T(0.5);
			}
		}
		matrix<T> a = m;
		jacobi<T> Jacobi(n);
		matrix<T> v(n,n);
		vector<T> d(n,0);
		if( !Jacobi(a,d,v) )
		{
			std::cerr << "Couldn't perform jacobi!" << std::endl;
			continue;
		}
		Jacobi.eigsrt(d,v);
		//std::cerr << "m=" << m << std::endl;
		//std::cerr << "d=" << d << std::endl;
	}
}

YOCTO_UNIT_TEST_IMPL(jacobi)
{
	test_jacobi<float>();
	test_jacobi<double>();
}
YOCTO_UNIT_TEST_DONE()

