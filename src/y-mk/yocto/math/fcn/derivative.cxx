#include "yocto/math/fcn/derivative.hpp"
#include "yocto/math/dat/extrapolate.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/ztype.hpp"


namespace yocto
{
	namespace math
	{
		
		template <>
		const real_t derivative<real_t>:: hdiv_ = REAL(1.4);
		
#if defined(_MSC_VER)
		// init of wksp_
#pragma warning ( disable : 4351 )
#endif
		template <>
		derivative<real_t>:: derivative() throw() :
		x_(),
		y_(),
		c_(),
		d_(),
		wksp_()
		{
			
			real_t *p = wksp_;
			x_.reset( p, kmax_ ); p += kmax_;
			y_.reset( p, kmax_ ); p += kmax_;
			c_.reset( p, kmax_ ); p += kmax_;
			d_.reset( p, kmax_ ); p += kmax_;
			
			x_[ 1 ] = 1;
			for( size_t i= 2; i <= kmax_; ++i)
			{
				x_[i] = x_[i-1] / hdiv_;
			}
			//std::cerr << "x=" << x_ << std::endl;
			
		}
		
		template <>
		derivative<real_t>:: ~derivative() throw()
		{
			
		}
		
		template <>
		real_t derivative<real_t>:: operator()(  numeric<real_t>::function &proc, real_t x, real_t h )
		{
			
			static const real_t hmin = numeric<real_t>::tiny * Pow( hdiv_, real_t(kmax_) );
			static const real_t dtol = Sqrt( numeric<real_t>:: ftol );
			
			h = max_of<real_t>( Fabs(h), hmin );
			
		INI:
			{
				//--------------------------------------------------------------
				// sanity check on h
				//--------------------------------------------------------------
				real_t one_h = h;
				real_t two_h = one_h + one_h;
				
				//--------------------------------------------------------------
				// first step
				//--------------------------------------------------------------
				real_t dy_old = Fabs( y_[1] = ( proc(x+one_h) - proc(x-one_h) ) / two_h );
				real_t dy_new = 0;
				
				//--------------------------------------------------------------
				// following steps
				//--------------------------------------------------------------
				for( size_t k=2; k <= kmax_; ++k )
				{
					one_h /= hdiv_;
					two_h  = one_h + one_h;
					y_[k] = ( proc(x+one_h) - proc(x-one_h) ) / two_h ;
					const real_t ans = extrapolate<real_t>::get( k, x_, y_, dy_new, c_, d_ );
					dy_new           = Fabs( dy_new );
					//std::cerr << "@" << k <<": dydx(" <<  x << ")=" << ans << " | err=" << dy_new << std::endl;
					
					//----------------------------------------------------------
					// check not too much divergence
					//----------------------------------------------------------
					if( dy_new > (dy_old+dy_old) )
					{
						//std::cerr << "divergence" << std::endl;
						goto END;
					}
					
					
					//----------------------------------------------------------
					// check convergence
					//----------------------------------------------------------
					if( dy_new <= Fabs( dtol * y_[k] ) )
					{
						return ans;
					}
					
					dy_old = dy_new;
					
				}
				//std::cerr << "no convergence" << std::endl;
			END:
				h *= REAL(0.5);
				goto INI;
			}
			
		}
		
		
		//======================================================================
		//
		// gradient
		//
		//======================================================================
		namespace 
		{
			struct _grad_t
			{
				numeric<real_t>::scalar_field *pF;
				array<real_t>                 *pX;
				size_t                         iv;
				
				real_t eval( real_t x )
				{
					assert( pF );
					assert( pX );
					assert( iv <= pX->size() );
					assert( iv > 0 );
					array<real_t>                 &X = *pX;
					numeric<real_t>::scalar_field &F = *pF;
					const real_t xsav = X[iv];
					X[iv] = x;
					try
					{
						const real_t ans = F(X);
						X[iv] = xsav;
						return ans;
					}
					catch(...)
					{
						X[iv] = xsav;
						throw;
					}
				}
			};
			
		}
		
		template <>
		void derivative<real_t>:: gradient( array<real_t> &grad, numeric<real_t>::scalar_field &F, const array<real_t> &x, real_t h )
		{
			const size_t nvar = grad.size();
			assert( x.size() == nvar );
			_grad_t _G = { &F, (array<real_t> *)&x, 0 };
			numeric<real_t>::function G( & _G, & _grad_t:: eval );
			size_t &i  = _G.iv;
			for(  i=1; i <= nvar; ++i )
			{
				grad[i] =  (*this)( G, x[i], h );
			}
		}
		
		

		
		
	}
	
}
