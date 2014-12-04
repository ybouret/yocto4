#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/explicit/rkck.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
			
			template <>
			RKCK<real_t>:: ~RKCK() throw()
			{
			}
			
			template <>
			RKCK<real_t>:: RKCK() :
            explicit_step<real_t>(6),
			ytmp( next_array() ),
			ak2(  next_array()   ),
			ak3(  next_array()   ),
			ak4(  next_array()   ),
			ak5(  next_array()   ),
			ak6(  next_array()   )
			{
			}
			
			template <>
			void RKCK<real_t>:: operator()(array<real_t>        &yout,
										   array<real_t>        &yerr,
										   equation             &drvs,
										   const real_t          x,
										   const real_t          h,
										   const array<real_t>  &y,
										   const array<real_t>  &dydx,
                                           callback             *cb
										   )
			{
                static const real_t a2  = REAL(0.2), a3=REAL(0.3), a4=REAL(0.6), a5=REAL(1.0), a6=REAL(0.875);
				static const real_t b21 = REAL(0.2);
				static const real_t b31 = REAL(3.0)/REAL(40.0),        b32=REAL(9.0)/REAL(40.0);
				static const real_t b41 = REAL(0.3),                   b42=REAL(-0.9),              b43=REAL(1.2);
				static const real_t b51 = REAL(-11.0)/REAL(54.0),      b52=REAL(2.5),               b53 = REAL(-70.0)/REAL(27.0),  b54=REAL(35.0)/REAL(27.0);
				static const real_t b61 = REAL(1631.0)/REAL(55296.0),  b62=REAL(175.0)/REAL(512.0), b63=REAL(575.0)/REAL(13824.0), b64=REAL(44275.0)/REAL(110592.0), b65=(253.0)/REAL(4096.0);
				static const real_t c1=REAL(37.0)/REAL(378.0), c3=REAL(250.0)/REAL(621.0), c4=REAL(125.0)/REAL(594.0), c6=REAL(512.0)/REAL(1771.0);
				static const real_t dc1=c1-REAL(2825.0)/REAL(27648.0), dc3=c3-REAL(18575.0)/REAL(48384.0), dc4=c4-REAL(13525.0)/REAL(55296.0), dc5 = REAL(-277.0)/REAL(14336.0), dc6=c6-REAL(0.25);
                
                
				const size_t n = y.size();
				
                //______________________________________________________________
                //
                //assuming y is valid w.r.t the callback
                //______________________________________________________________
#define RKCK_CHK(A) xx=x+A*h; if (cb) (*cb)(ytmp,xx)
                
				for (size_t i=n;i>0;--i)
                {
					ytmp[i]=y[i]+b21*h*dydx[i];
                }
                
                real_t RKCK_CHK(a2);
                drvs(ak2,xx,ytmp);
                
				for (size_t i=n;i>0;--i)
                {
					ytmp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
                }
                
                RKCK_CHK(a3);
				drvs(ak3,xx,ytmp);
				
				for (size_t i=n;i>0;--i)
                {
					ytmp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
                }
                
                RKCK_CHK(a4);
				drvs(ak4,xx,ytmp);
				
				for (size_t i=n;i>0;--i)
                {
					ytmp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
                }
                
                RKCK_CHK(a5);
				drvs(ak5,xx,ytmp);
				
				for(size_t i=n;i>0;--i)
                {
					ytmp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
                }
                
                RKCK_CHK(a6);
				drvs(ak6,xx,ytmp);
				
				for (size_t i=n;i>0;--i)
                {
					yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);
                }
                
                if(cb)
                {
                    (*cb)(yout,x+h);
                }
                
				for (size_t i=n;i>0;--i)
                {
					yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);
                }
				
			}
			
			
		}
		
	}
	
}

