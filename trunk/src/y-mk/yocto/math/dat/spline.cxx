#include "yocto/math/dat/spline.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto {

	namespace math {

		#if defined(__GNUC__)
		template <>
		#endif
		spline<real_t>::boundary:: ~boundary() throw()
		{
		}

		#if defined(__GNUC__)
		template <>
		#endif
		spline<real_t>::boundary:: boundary( const bool natural, const real_t slope ) throw() :
				is_natural( natural ),
				derivative( slope )
		{
		}

		#if defined(__GNUC__)
		template <>
		#endif
		spline<real_t>::boundary:: boundary( const boundary &b ) throw() :
				is_natural( b.is_natural ),
				derivative( b.derivative )
		{
		}

		template <>
		spline<real_t>:: ~spline() throw()
		{
		}

		template <>
		spline<real_t>:: spline( const array<real_t> &x, const array<real_t> &y, const boundary &lo, const boundary &hi ) :
				x_( NULL ),
				y_( NULL ),
				y2_( NULL ),
				n_( x.size() ),
				xlo_(0),
				xhi_(0),
				ylo_(0),
				yhi_(0),
				w_( 3*n_, 0 )
		{
			assert( x.size()>=2 );
			assert( y.size()>=2 );
			assert( x.size() == y.size() );

			//-- prepare arrays
			x_  = w_(-1);
			y_  = x_ + n_;
			y2_ = y_ + n_;

			//-- temporary
			vector<real_t> u( n_-1, 0 );

			//-- initialize arrays
			for( size_t i=n_;i>0;--i)
			{
				x_[i]  = x[i];
				y_[i]  = y[i];
				y2_[i] = 0;
			}

			*(real_t *)&xlo_ = x_[1];
			*(real_t *)&ylo_ = y_[1];
			*(real_t *)&xhi_ = x_[n_];
			*(real_t *)&yhi_ = y_[n_];

			//------------------------------------------------------------------
			// lo
			//------------------------------------------------------------------
			if( lo.is_natural )
				y2_[1] = u[1] = 0.0;
			else
			{
				y2_[1] = -REAL(0.5);
				const real_t dx = x_[2] - x_[1];
				u[1] = (REAL(3.0) / dx) * ( (y_[2] - y_[1]) / dx - lo.derivative );
			}

			//------------------------------------------------------------------
			// mid
			//------------------------------------------------------------------
			for( size_t i=2; i < n_; ++i )
			{
				const real_t sig = ( x_[i] - x_[i-1] ) / ( x_[i+1] - x_[i-1] );
				const real_t p   = sig * y2_[i-1] + REAL(2.0);
				const real_t tmp = (y_[i+1] - y[i])/(x_[i+1]-x_[i]) - (y_[i] - y[i-1])/(x_[i]-x_[i-1]);

				y2_[i] = ( sig - REAL(1.0) ) / p;
				u[i]   = ( REAL(6.0) * tmp / (x_[i+1]-x_[i-1]) - sig * u[i-1]) / p;

			}

			//------------------------------------------------------------------
			// hi
			//------------------------------------------------------------------
			real_t qn = 0, un = 0;
			if( !hi.is_natural )
			{
				const real_t dx = x_[n_] - x_[n_-1];
				qn = REAL(0.5);
				un = ( REAL(3.0) / dx ) * ( hi.derivative - (y_[n_] - y_[n_-1] ) / dx );
			}
			y2_[n_] = ( un - qn * u[n_-1] ) / ( qn * y2_[n_-1] + REAL(1.0) );
			for( size_t k=n_-1;k>0;--k)
			{
				y2_[k] = y2_[k] * y2_[k+1] + u[k];
			}

		}


		template <>
		real_t spline<real_t>:: operator() ( real_t x ) const throw()
		{
			if( x <= xlo_ )
				return ylo_;
			else {

				if( x >= xhi_ )
					return yhi_;
				else
				{
					size_t klo = 1, khi = n_;
					while( (khi-klo)> 1 )
					{
						const size_t k = (klo+khi)>>1;
						if( x_[k] > x )
							khi = k;
						else
							klo = k;
					}
					const real_t h = x_[khi] - x_[klo];
					const real_t a = (x_[khi] - x)/h;
					const real_t b = (x - x_[klo])/h;
					return (a * y_[klo]) + (b * y_[khi]) + ( (a*a*a-a) * y2_[klo] + (b*b*b-b) * y2_[khi] ) * (h*h) / REAL(6.0);
				}
			}
		}

	}

}
