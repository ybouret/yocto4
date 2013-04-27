#include "yocto/math/dat/spline.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/tridiag.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/code/hsort.hpp"

namespace yocto {
    
	namespace math {
        
        template <>
        void spline<real_t>:: compute(spline_type          type,
                                      const array<real_t> &x,
                                      const array<real_t> *y_tab,
                                      array<real_t>       *y2_tab,
                                      const real_t        *ls_tab,
                                      const real_t        *rs_tab,
                                      const size_t         ns)
        {
            assert(ns>0);
            assert(y_tab);
            assert(y2_tab);
            const size_t n = x.size(); assert(n>1);
            
            //==================================================================
            // create the system matrix
            //==================================================================
            auto_ptr< tridiag_base<real_t> > pM;
            
            switch( type )
            {
                case spline_periodic:
                    pM.reset( new ctridiag<real_t>(n) );
                    break;
                    
                default:
                    pM.reset( new tridiag<real_t>(n) );
                    break;
            }
            
            //==================================================================
            // fill the core matrix
            //==================================================================
            tridiag_base<real_t> &M = *pM;
            
            for(size_t i=2;i<n;++i)
            {
                const size_t im  = i-1;
                const size_t ip  = i+1;
                const real_t xm  = x[im];
                const real_t xi  = x[i];
                const real_t xp  = x[ip];
                
                const real_t dxp = xp - xi; assert(dxp>0);
                const real_t dxm = xi - xm; assert(dxm>0);
                
                M.a[i] = dxm/REAL(6.0);
                M.b[i] = (xp - xm)/REAL(3.0);
                M.c[i] = dxp/REAL(6.0);
                for(size_t j=0;j<ns;++j)
                {
                    const array<real_t> &y   = y_tab[j]; assert( n == y.size()  );
                    array<real_t>       &y2 = y2_tab[j]; assert( n == y2.size() );
                    const real_t yi = y[i];
                    y2[i]  = (y[ip]-yi)/dxp - (yi-y[im])/dxm;
                }
            }
            
            const size_t nm  = n-1;
            const real_t dx1 = x[2] - x[1];
            const real_t dxn = x[n] - x[nm];
            
            //==================================================================
            // fill the side matrix/right hand vector
            //==================================================================
            switch(type)
            {
                case spline_natural:
                    M.b[1] = 1;
                    M.b[n] = 1;
                    for(size_t j=0;j<ns;++j)
                    {
                        array<real_t> &y2 = y2_tab[j];
                        y2[1] = y2[n] = 0;
                    }
                    break;
                    
                case spline_periodic:
                    M.b[1] = M.b[n] = (dx1+dxn) / REAL(3.0);
                    M.c[n] = dxn / REAL(6.0);
                    M.a[1] = dx1 / REAL(6.0);
                    for(size_t j=0;j<ns;++j)
                    {
                        const array<real_t> &y   = y_tab[j];
                        array<real_t>       &y2 = y2_tab[j];
                        const real_t yh  = REAL(0.5) * (y[1]+y[n]);
                        const real_t dy1 = y[2] - yh;
                        const real_t dyn = yh   - y[nm];
                        y2[n]  = - ( y2[1] = dy1/dx1 - dyn/dxn);
                    }
                    break;
                    
                case spline_tangent_left:
                    assert(ls_tab!=NULL);
                    M.b[1] = dx1/REAL(3.0);
                    M.c[1] = dx1/REAL(6.0);
                    for(size_t j=0;j<ns;++j)
                    {
                        const array<real_t> &y   = y_tab[j];
                        array<real_t>       &y2 = y2_tab[j];
                        const real_t dy1 = y[2] - y[1];
                        y2[1] =  dy1/dx1 - ls_tab[j];
                    }
                    break;
                    
                case spline_tangent_right:
                    assert(rs_tab!=NULL);
                    M.b[n] = dxn / REAL(3.0);
                    M.a[n] = dxn / REAL(6.0);
                    for(size_t j=0;j<ns;++j)
                    {
                        const array<real_t> &y  = y_tab[j];
                        array<real_t>       &y2 = y2_tab[j];
                        const real_t dyn = y[n] - y[nm];
                        y2[n]  = rs_tab[j] - dyn/dxn;
                    }
                    break;
                    
                case spline_tangent_both:
                    assert(ls_tab!=NULL);
                    assert(rs_tab!=NULL);
                    M.b[1] = dx1/REAL(3.0);
                    M.c[1] = dx1/REAL(6.0);
                    M.b[n] = dxn / REAL(3.0);
                    M.a[n] = dxn / REAL(6.0);
                    for(size_t j=0;j<ns;++j)
                    {
                        const array<real_t> &y   = y_tab[j];
                        array<real_t>       &y2 = y2_tab[j];
                        const real_t dy1 = y[2] - y[1];
                        const real_t dyn = y[n] - y[nm];
                        y2[1] =  dy1/dx1 - ls_tab[j];
                        y2[n] = rs_tab[j] - dyn/dxn;
                    }
                    break;
                    
                    
            }
            
            //==================================================================
            // solve all the y2
            //==================================================================
            for(size_t j=0;j<ns;++j)
            {
                array<real_t> &y2 = y2_tab[j];
                if( !M.solve(y2) )
                    throw exception("spline::compute(SINGULAR)");
            }
            
            
        }
        
        template <>
        void spline<real_t>:: eval(real_t              *Y,
                                   const size_t         ns,
                                   real_t               X,
                                   const array<real_t> &x,
                                   const array<real_t> *y_tab,
                                   const array<real_t> *y2_tab,
                                   const real_t        *width)
        {
            assert(ns>0);
            assert(Y);
            assert(y_tab);
            assert(y2_tab);
            const size_t n   = x.size();
            const real_t xlo = x[1];
            const real_t xup = x[n]; assert(xlo<xup);
            
            //------------------------------------------------------------------
            //-- take care of X
            //------------------------------------------------------------------
            if( width != 0 )
            {
                const real_t W = (*width); assert(W>0);
                while( X>xup ) X -= W;
                while( X<xlo ) X += W;
            }
            else
            {
                if( X <= xlo )
                {
                    for(size_t j=0;j<ns;++j) { Y[j] = y_tab[j][1]; }
                    return;
                }
                if( X >= xup )
                {
                    for(size_t j=0;j<ns;++j) { Y[j] = y_tab[j][n]; }
                    return;
                }
            }
            
            //------------------------------------------------------------------
            //-- effective evaluation
            //------------------------------------------------------------------
            size_t klo = 1, khi = n;
            while( (khi-klo)> 1 )
            {
                const size_t k = (klo+khi)>>1;
                if( x[k] > X )
                    khi = k;
                else
                    klo = k;
            }
            const real_t h  = x[khi] - x[klo];
            const real_t a  = (x[khi] - X)/h;
            const real_t b  = (X - x[klo])/h;
            const real_t H  = h*h/REAL(6.0);
            const real_t C  = (a*a*a-a);
            const real_t D  = (b*b*b-b);
            
            //return (a * y[klo]) + (b * y[khi]) + ( (a*a*a-a) * y2[klo] + (b*b*b-b) * y2[khi] ) * (h*h) / REAL(6.0);
            for(size_t j=0;j<ns;++j)
            {
                const array<real_t> &y  =  y_tab[j];
                const array<real_t> &y2 = y2_tab[j];
                Y[j] =  (a * y[klo]) + (b * y[khi]) + ( C*y2[klo]+D*y2[khi] ) * H;
            }
        }
        
#if 0
        template <>
        spline<real_t>:: ~spline() throw()
        {
        }
        
        
        template <>
        void spline<real_t>:: compute(const boundaries &b)
        {
            (spline_type &)type = b.type;
            
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
                const size_t im  = i-1;
                const size_t ip  = i+1;
                const real_t xm  = x[im];
                const real_t xi  = x[i];
                const real_t xp  = x[ip];
                
                const real_t dxp = xp - xi;
                const real_t dxm = xi - xm;
                
                M.a[i] = dxm/REAL(6.0);
                M.b[i] = (xp - xm)/REAL(3.0);
                M.c[i] = dxp/REAL(6.0);
                const real_t yi = y[i];
                y2[i]  = (y[ip]-yi)/dxp - (yi-y[im])/dxm;
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
                    y2[n]  = - ( y2[1] = dy1/dx1 - dyn/dxn);
                    M.b[1] = M.b[n] = (dx1+dxn) / REAL(3.0);
                    M.c[n] = dxn / REAL(6.0);
                    M.a[1] = dx1 / REAL(6.0);
                    break;
            }
            if(!M.solve(y2) )
                throw exception("singular spline");
            
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
            // compute spline
            //==================================================================
            compute(b);
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
        
#endif
        
	}
    
}
