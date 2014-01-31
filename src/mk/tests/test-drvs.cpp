#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/math/fcn/derivative.hpp"
#include "yocto/ios/ocstream.hpp"
#include <typeinfo>

using namespace yocto;
using namespace math;

namespace {
	
	template <typename T>
	static inline T F(T x )
	{
		return Sin(x);
	}
	
	template <typename T>
	class dummy
	{
	public:
		dummy()   : drvs(), func( cfunctor( F<T> ) ) {}
		~dummy() throw() {}
		
		T compute( T x )
		{
			const T ans = drvs( func, x, 1e-4 );
			return ans;
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
		derivative<T>                 drvs;
		typename numeric<T>::function func;
	};
	
	template <typename T>
	static inline void test_drvs()
	{
		std::cerr << "drvs/intg" << std::endl;
		const T      xmax = numeric<T>::two_pi;
		const size_t N = 128;
		dummy<T>     dum;
        typename numeric<T>::function dF( &dum, & dummy<T>::compute );
        integrator<T> intg;
		const string out = string("drvs-") + typeid(T).name() + ".dat";
		ios::ocstream fp( out, false );
		T z = 0;
		fp("%g %g\n", 0.0, z );
		for( size_t i=1; i <= N; ++i )
		{
			const T x1 = (i*xmax)/N;			
			const T x0 = ( (i-1) * xmax)/N; 
			std::cerr << x0 << " -> " << x1 << std::endl;
			const T y = intg( x0, x1, dF, 2*numeric<T>::sqrt_epsilon );
			z += y;
			std::cerr << "\t" << z << " / " << F<T>(x1) << std::endl;
			fp("%g %g\n", x1, z );
		}
	}
}

YOCTO_UNIT_TEST_IMPL(drvs)
{
	//test_drvs<float>();
	test_drvs<double>();
}
YOCTO_UNIT_TEST_DONE()
