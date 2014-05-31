#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/math/fcn/drvs.hpp"
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
    
    static unsigned Counts = 0;
    template <typename T>
    static inline T Cos2( T x )
    {
        ++Counts;
        return Cos( x*x );
    }
    
}

#include "yocto/math/fcn/drvs.hpp"

YOCTO_UNIT_TEST_IMPL(drvs)
{
	//test_drvs<float>();
    //test_drvs<double>();
    
    derivative<double> drvs;
    
    const double xmax = 10;
    const double step = 0.1;
    numeric<double>::function F = cfunctor( Cos2<double> );
    {
        double err = 0;
        ios::ocstream fp("diff.dat",false);
        for( double x=0; x <= xmax; x += step)
        {
            Counts = 0;
            const double dFdx = drvs.diff(F, x, 0.001, err);
            fp("%g %g %g %u %g\n", x, F(x), dFdx, Counts, err);
        }
    }
    
    {
        ios::ocstream fp("diff2.dat",false);
        for( double x=0; x <= xmax; x += step)
        {
            Counts = 0;
            const double dFdx = drvs(F, x, 0.001);
            fp("%g %g %g %u\n", x, F(x), dFdx, Counts);
        }

    }
    
}
YOCTO_UNIT_TEST_DONE()

