
#include "yocto/math/ode/rk45.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

#include <iostream>

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			
			template <>
			rk45<real_t>:: ~rk45() throw()
			{
			}
			
			template <>
			rk45<real_t>:: rk45() :
			control<real_t>(2),
			yerr( next_array() ),
			ytmp( next_array() )
			{
			}
			
			template <>
			void rk45<real_t>::operator()(step<real_t>        &forward,
										  array<real_t>       &y, 
										  const array<real_t> &dydx,
										  equation            &drvs,
										  real_t              &x,
										  const real_t         h_try,
										  real_t              &h_did,
										  real_t              &h_next,
										  const array<real_t> &yscal,
										  const real_t         eps)
			{
				static const real_t SAFETY = REAL(0.9);
				static const real_t PGROW  = REAL(-0.2);
				static const real_t PSHRNK = REAL(-0.25);
				//The value ERRCON equals (5/SAFETY) raised to the power (1/PGROW), see use below	
				static const real_t ERRCON = Pow( REAL(5.0)/SAFETY, REAL(1.0)/PGROW);
				static const real_t DEC    = REAL(0.1);
				static const real_t INC    = REAL(5.0);
				
				
				//--------------------------------------------------------------
				// sanity check
				//--------------------------------------------------------------
				assert( common_size() >= y.size() );
				
				
				//--------------------------------------------------------------
				// initialize
				//--------------------------------------------------------------
				const size_t n        = y.size();
				real_t       h        = h_try; 
				real_t       errmax   = 0;
				const bool   positive = h >= 0;
				
				
				
				for (;;) {
					//----------------------------------------------------------
					// embbeded step+error
					//----------------------------------------------------------
					forward( ytmp, yerr, drvs, x, h, y, dydx );
					
					//----------------------------------------------------------
					// accuracy evaluation
					//----------------------------------------------------------
					errmax=0;                           
					for (size_t i=n; i>0; --i) 
						errmax= max_of<real_t>(errmax,Fabs(yerr[i]/yscal[i]));
					errmax /= eps;
					if (errmax <= REAL(1.0) )
						break; // ==> success
					
					//----------------------------------------------------------
					// shall retry
					//----------------------------------------------------------
					const real_t htemp=SAFETY*h*Pow(errmax,PSHRNK);
					h=(positive ? max_of<real_t>(htemp,DEC*h) : min_of<real_t>(htemp,DEC*h));
					
					//----------------------------------------------------------
					// control overflow
					//----------------------------------------------------------
					const real_t xnew=x+h;
					if( (positive && xnew <= x) || (!positive && xnew >= x ) ) 
						throw libc::exception( ERANGE, "stepsize underflow in rk45");
				}
				
				//--------------------------------------------------------------
				// sucess: finalize
				//--------------------------------------------------------------
				x += (h_did=h);
				
				if (errmax > ERRCON) 
					h_next=SAFETY*h*Pow(errmax,PGROW);
				else 
					h_next=INC*h;
				
				for (size_t i=n;i>0;--i) 
					y[i]=ytmp[i];
			}
			
			
		}
		
	}
	
}
