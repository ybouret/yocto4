#include "yocto/utest/run.hpp"
#include "yocto/math/ode/rkck.hpp"
#include "yocto/math/ode/rk45.hpp"
#include "yocto/math/ode/solver.hpp"

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
		explicit dummy()
		{
		}
		
		virtual ~dummy() throw()
		{
		}
		
		void eval1( array<T> &dydx, T , const array<T> &y )
		{
			dydx[1] = -y[1];
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
	ode::rkck<T>                 step;
	ode::rk45<T>                 ctrl;
	typename ode::field<T>::type drvs( &dum, & dummy<T>::eval1 );
	ode::solver<T>               dsolve;
	
	const size_t nv = 1;
	step.prepare( nv );
	ctrl.prepare( nv );
	dsolve.prepare( nv );
	vector<T> y( nv, 0 );
		
	y[1] = 1;
	T h  = 0.1;
	T x  = 0;
	T dx = 0.2;
	
	ios::ocstream fp( "exp.txt", false );
	for( ; x <= 5; x += 0.2 )
	{
		fp("%g %g\n", x, y[1] );
		dsolve( drvs, ctrl, step, y, x, x+dx, h, 0, 1e-7 );
	}
	fp("%g %g\n", x, y[1] );

	
}


YOCTO_UNIT_TEST_IMPL(ode)
{
	perform_ode<double>();
	
}
YOCTO_UNIT_TEST_DONE()