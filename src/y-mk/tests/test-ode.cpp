#include "yocto/utest/run.hpp"
#include "yocto/math/ode/rkck.hpp"
#include "yocto/sequence/vector.hpp"

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
	typename ode::field<T>::type drvs( &dum, & dummy<T>::eval1 );
	
	const size_t nv = 1;
	step.prepare( nv );
	vector<T> y( nv, 0 );
	vector<T> dydx( nv, 0 );
	vector<T> yout(nv,0);
	vector<T> yerr(nv,0);
	
	y[1] = 1;
	drvs( dydx, 0, y );
	step( yout, yerr, drvs, 0, 0.1, y, dydx );
	
	
}


YOCTO_UNIT_TEST_IMPL(ode)
{
	perform_ode<double>();
	
}
YOCTO_UNIT_TEST_DONE()