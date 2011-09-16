#include "yocto/utest/run.hpp"
#include "yocto/math/ode/drvck.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

#include <typeinfo>


using namespace yocto;
using namespace math;


namespace {
	
	template <typename T>
	class dummy 
	{
	public:
		explicit dummy() : lam(0.1), w(1)
		{
		}
		
		virtual ~dummy() throw()
		{
		}
		
		T lam;
		T w;
		
		void eval1( array<T> &dydx, T , const array<T> &y )
		{
			dydx[1] = -lam*y[1];
		}
		
		void eval2( array<T> &dydx, T , const array<T> &y )
		{
			dydx[1] = y[2];
			dydx[2] = -(w*w) * y[1];
		}
		
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
	
}

template <typename T>
static inline
void perform_ode( )
{
	dummy<T>                     dum;
	typename ode::drvck<T>::type odeint;
	
	typename ode::field<T>::type drvs1( &dum, & dummy<T>::eval1 );
	typename ode::field<T>::type drvs2( &dum, & dummy<T>::eval2 );
	
	{
		const size_t nv = 1;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		T h  = 0.1;
		T x  = 0;
		T dx = 0.2;
		
		ios::ocstream fp( "exp.txt", false );
		for( ; x <= 50; x += 0.2 )
		{
			fp("%g %g\n", x, y[1] );
			odeint( drvs1, y, x, x+dx, h );
		}
		fp("%g %g\n", x, y[1] );
	}
	
	{
		const size_t nv = 2;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		y[2] = 0;
		T h  = 0.1;
		T x  = 0;
		T dx = 0.2;
		
		ios::ocstream fp( "cos.txt", false );
		for( ; x <= 50; x += 0.2 )
		{
			fp("%g %g\n", x, y[1] );
			odeint( drvs2, y, x, x+dx, h );
		}
		fp("%g %g\n", x, y[1] );
	}
		
}


YOCTO_UNIT_TEST_IMPL(ode)
{
	perform_ode<double>();
	
}
YOCTO_UNIT_TEST_DONE()