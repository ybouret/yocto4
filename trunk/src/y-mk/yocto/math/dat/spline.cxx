#include "yocto/math/dat/spline.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/tridiag.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/code/hsort.hpp"

namespace yocto {
    
	namespace math {
        
#if 0
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
#endif
        template <>
        spline<real_t>:: ~spline() throw()
        {
        }
        
        template <>
        spline<real_t>:: spline( const array<real_t> &X, const array<real_t> &Y, const boundaries &b ) :
        type(b.type),
        arrays(3),
        x(  arrays.next_array() ),
        y(  arrays.next_array() ),
        y2( arrays.next_array() ),
        n( X.size() ),
        xlo(0),
        xup(0),
        width(0)
        {
            assert( X.size() == Y.size() );
            assert( X.size() > 1 );
            
            //==================================================================
            // prepare data
            //==================================================================
            arrays.prepare(n);
            for(size_t i=1; i <= n; ++i )
            {
                x[i] = X[i];
                y[i] = Y[i];
            }
            co_hsort(x,y);
            (real_t&)xlo   = x[1];
            (real_t&)xup   = x[n];
            (real_t&)width = xup - xlo;
            
            //==================================================================
            // create the system matrix
            //==================================================================
            auto_ptr< tridiag_base<real_t> > pM;
            
            switch( b.type )
            {
                case spline_cyclic:
                    pM.reset( new ctridiag<real_t>(n) );
                    y[1] = y[n] = (y[1]+y[n])/2;
                    break;
                    
                case spline_regular:
                    pM.reset( new tridiag<real_t>(n) );
                    break;
            }
            
            //==================================================================
            // fill the core matrix
            //==================================================================
            tridiag_base<real_t> &M = *pM;
            
            for(size_t i=2;i<n;++i)
            {
                const real_t xm  = x[i-1];
                const real_t xi  = x[i];
                const real_t xp  = x[i+1];
                
                const real_t dxp = xp - xi;
                const real_t dxm = xi - xm;
                
                M.a[i] = dxm/REAL(6.0);
                M.b[i] = (xp - xm)/REAL(3.0);
                M.c[i] = dxp/REAL(6.0);
                const real_t yi = y[i];
                y2[i]  = (y[i+1]-yi)/dxp - (yi-y[i-1])/dxm;
            }
            
            const real_t dx1 = x[2] - x[1];
            const real_t dy1 = y[2] - y[1];
            const real_t dxn = x[n] - x[n-1];
            const real_t dyn = y[n] - y[n-1];
            
            switch( b.type )
            {
                case spline_regular:
                    if( b.lower.natural )
                    {
                        y2[1]  = 0;
                        M.b[1] = 1;
                    }
                    else
                    {
                        M.b[1] = dx1/REAL(3.0);
                        M.c[1] = dx1/REAL(6.0);
                        y2[1] =  dy1/dx1 - b.lower.slope;
                    }
                    
                    if( b.upper.natural )
                    {
                        y2[n]  = 0;
                        M.b[n] = 1;
                    }
                    else
                    {
                        M.b[n] = dxn / REAL(3.0);
                        M.a[n] = dxn / REAL(6.0);
                        y2[n]  = b.upper.slope - dyn/dxn;
                    }
                    break;
                    
                case spline_cyclic:
                    y2[n] = - ( y2[1] = dy1/dx1 - dyn/dxn);
                    M.b[1] = M.b[n] = (dx1+dxn) / REAL(3.0);
                    M.c[n] = dxn / REAL(6.0);
                    M.a[1] = dx1 / REAL(6.0);
                    break;
            }
            if(!M.solve(y2) )
                throw exception("singular spline");
        }
        
        template<>
        real_t spline<real_t>:: operator()(real_t X) const throw()
        {
            switch( type )
            {
                case spline_cyclic:
                    while( X < xlo ) X += width;
                    while( X > xup ) X -= width;
                    break;
                    
                case spline_regular:
                    if( X <= xlo ) return y[1];
                    if( X >= xup ) return y[n];
                    break;
            }
            size_t klo = 1, khi = n;
            while( (khi-klo)> 1 )
            {
                const size_t k = (klo+khi)>>1;
                if( x[k] > X )
                    khi = k;
                else
                    klo = k;
            }
            const real_t h = x[khi] - x[klo];
            const real_t a = (x[khi] - X)/h;
            const real_t b = (X - x[klo])/h;
            return (a * y[klo]) + (b * y[khi]) + ( (a*a*a-a) * y2[klo] + (b*b*b-b) * y2[khi] ) * (h*h) / REAL(6.0);
        }
        
        
	}
    
}
