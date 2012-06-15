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
		
		void eval3( array<T> &dydx, T , const array<T> &y )
		{
			const T a = 3, b = 2, c = 3, d = 2;
			const T X = y[1];
			const T Y = y[2];
			dydx[1] =  a*X - b * X * Y;
			dydx[2] = -c*Y + d * X * Y;
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
	DRIVER                       odeint(1e-4);
	
	typename ode::field<T>::type drvs1( &dum, & dummy<T>::eval1 );
	typename ode::field<T>::type drvs2( &dum, & dummy<T>::eval2 );
	typename ode::field<T>::type drvs3( &dum, & dummy<T>::eval3 );

	dum.calls = 0;
	{
		const size_t nv = 1;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		T h  = 0.1;
		T x  = 0;
		const T dx = 0.2;
		
		ios::ocstream fp( drvid + "-exp.dat", false );
		for( ; x <= 50; x += dx )
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
		const T dx = 0.2;
		
		ios::ocstream fp( drvid + "-cos.dat", false );
		for( ; x <= 50; x += dx )
		{
			fp("%g %g\n", x, y[1] );
			odeint( drvs2, y, x, x+dx, h );
		}
		fp("%g %g\n", x, y[1] );
	}
	std::cerr << drvid << " cos => " << dum.calls << " calls" << std::endl;

	dum.calls = 0;
	{
		const size_t nv = 2;
		odeint.start( nv );
		vector<T> y( nv, 0 );
		
		y[1] = 1;
		y[2] = 2;
		T h  = 0.1;
		T x  = 0;
		const T dx = 0.2;
		
		ios::ocstream fp( drvid + "-lv.dat", false );
		for( ; x <= 50; x += dx )
		{
			fp("%g %g %g\n", x, y[1], y[2] );
			odeint( drvs2, y, x, x+dx, h );
		}
		fp("%g %g %g\n", x, y[1], y[2] );
	}
	std::cerr << drvid << " lv  => " << dum.calls << " calls" << std::endl;
	
		
}


YOCTO_UNIT_TEST_IMPL(ode)
{
	perform_ode<double, ode::drvck<double>::type >( "ck" );
	perform_ode<double, ode::drvdp<double>::type >( "dp" );

}
YOCTO_UNIT_TEST_DONE()

