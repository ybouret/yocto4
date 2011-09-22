#include "yocto/math/ode/rkdp.hpp"
#include "yocto/math/ztype.hpp"


namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
			template <> 
			rkdp<real_t>:: ~rkdp() throw()
			{
				
			}
			
			template <>
			rkdp<real_t>:: rkdp() :
			step<real_t>(7),
			ytmp( next_array() ),
			ak2(  next_array()   ),
			ak3(  next_array()   ),
			ak4(  next_array()   ),
			ak5(  next_array()   ),
			ak6(  next_array()   ),
			ak7(  next_array()   )
			{
			}
			
			template <>
			void rkdp<real_t>:: operator()(array<real_t>        &yout, 
										   array<real_t>        &yerr, 
										   equation             &drvs, 
										   const real_t          x,    
										   const real_t          h,    
										   const array<real_t>  &y,    
										   const array<real_t>  &dydx 
										   )
			{
				static const real_t a2  = 1.0/5.0, a3 = 3.0/10.0, a4 = 4.0/5.0, a5 = 8.0/9.0;// a6=1.0;
				static const real_t b21 = 1.0/5.0;
				static const real_t b31 = 3.0/40.0,       b32 =  9.0/40.0;
				static const real_t b41 = 44.0/45.0,      b42 = -56.0/15.0,      b43 = 32.0/9.0;
				static const real_t b51 = 19372.0/6561.0, b52 =	-25360.0/2187.0, b53 = 64448.0/6561.0,	    b54= -212.0/729.0;
				static const real_t b61 = 9017.0/3168.0,  b62 = -355.0/33.0,     b63 = 46732.0/5247.0,      b64 = 49.0/176.0,     b65 = -5103.0/18656.0;
				static const real_t c1  = 35.0/384.0,                            c3  = 500.0/1113.0,        c4  = 125.0/192.0,    c5  = -2187.0/6784.0,       c6=11.0/84.0;
				static const real_t dc1 = c1 - 5179.0/57600.0,	                 dc3 = c3 -	7571.0/16695.0,	dc4 = c4-393.0/640.0, dc5 = c5+92097.0/339200.0,  dc6 = c6 - 187.0/2100.0, dc7 = 0 - 1.0/40.0;

				const size_t n = y.size();
				
				for (size_t i=n;i>0;--i) 
					ytmp[i]=y[i]+b21*h*dydx[i];
				
				drvs(ak2,x+a2*h,ytmp); 
				for (size_t i=n;i>0;--i)
					ytmp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
				
				drvs(ak3,x+a3*h,ytmp); 
				
				for (size_t i=n;i>0;--i)
					ytmp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
				
				drvs(ak4,x+a4*h,ytmp);
				
				for (size_t i=n;i>0;--i)
					ytmp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
				
				drvs(ak5,x+a5*h,ytmp); 
				
				for (size_t i=n;i>0;--i)
					ytmp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
				
				const real_t x6 = x + h;
				drvs(ak6,x6,ytmp);
				
				for (size_t i=n;i>0;--i) 
					yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c5*ak5[i]+c6*ak6[i]);
				
				drvs( ak7,x6,yout);
				
				for (size_t i=n;i>0;--i)
					yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]+dc7*ak7[i]);
				
			}
			
		}
	}
	
}
