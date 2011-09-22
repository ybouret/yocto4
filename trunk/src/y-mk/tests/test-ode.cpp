#include "yocto/utest/run.hpp"
#include "yocto/math/ode/drvck.hpp"
#include "yocto/math/ode/drvdp.hpp"

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
		explicit dummy() : lam(0.1), w(1), calls(0)
		{
		}
		
		virtual ~dummy() throw()
		{
		}
		
		T lam;
		T w;
		size_t calls;
		
		void eval1( array<T> &dydx, T , const array<T> &y )
		{
			dydx[1] = -lam*y[1];
			++calls;
		}
		
		void eval2( array<T> &dydx, T , const array<T> &y )
		{
			dydx[1] = y[2];
			dydx[2] = -(w*w) * y[1];
			++calls;
		}
		
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
	
}

template <typename T,typename DRIVER>
static inline
void perform_ode( const string &drvid )
{
	dummy<T>                     dum;
	DRIVER                       odeint;
	
	typename ode::field<T>::type drvs1( &dum, & dummy<T>::eval1 );
	typename ode::field<T>::type drvs2( &dum, & dummy<T>::eval2 );
	
	dum.calls = 0;
	{
		const size_t nv = 1;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		T h  = 0.1;
		T x  = 0;
		T dx = 0.2;
		
		ios::ocstream fp( drvid + "-exp.txt", false );
		for( ; x <= 50; x += 0.2 )
		{
			fp("%g %g\n", x, y[1] );
			odeint( drvs1, y, x, x+dx, h );
		}
		fp("%g %g\n", x, y[1] );
	}
	std::cerr << drvid << " exp => " << dum.calls << " calls" << std::endl;
	
	
	dum.calls = 0;
	{
		const size_t nv = 2;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		y[2] = 0;
		T h  = 0.1;
		T x  = 0;
		T dx = 0.2;
		
		ios::ocstream fp( drvid + "-cos.txt", false );
		for( ; x <= 50; x += 0.2 )
		{
			fp("%g %g\n", x, y[1] );
			odeint( drvs2, y, x, x+dx, h );
		}
		fp("%g %g\n", x, y[1] );
	}
	std::cerr << drvid << " cos => " << dum.calls << " calls" << std::endl;

		
}


YOCTO_UNIT_TEST_IMPL(ode)
{
	perform_ode<double, ode::drvck<double>::type >( "ck" );
	perform_ode<double, ode::drvdp<double>::type >( "dp" );

}
YOCTO_UNIT_TEST_DONE()