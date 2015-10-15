#include "yocto/math/dat/spline.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/core/tridiag.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/math/types.hpp"

namespace yocto {
    
	namespace math {
        
        template <>
        void spline<real_t>:: compute(spline_type          type,
                                      const array<real_t> &x,
                                      const array<real_t> **ppy,
                                      array<real_t>       **ppy2,
                                      const real_t        *ls_tab,
                                      const real_t        *rs_tab,
                                      const size_t         ns)
        {
            assert(ns>0);
            assert(ppy);
            assert(ppy2);
            const size_t n = x.size();
            assert(n>=3);
            
            if( type != spline_periodic)
            {
                //==============================================================
                // create the system matrix
                //==============================================================
                TriDiag<real_t> M(n);
                
                //==============================================================
                // fill the core matrix
                //==============================================================
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
                        assert(ppy[j]);
                        assert(ppy2[j]);
                        const array<real_t> &y  = *ppy[j];  assert( n == y.size()  );
                        array<real_t>       &y2 = *ppy2[j]; assert( n == y2.size() );
                        const real_t yi = y[i];
                        y2[i]  = (y[ip]-yi)/dxp - (yi-y[im])/dxm;
                    }
                }
                
                const size_t nm  = n-1;
                const real_t dx1 = x[2] - x[1];
                const real_t dxn = x[n] - x[nm];
                
                //==============================================================
                // fill the side matrix/right hand vector
                //==============================================================
                switch(type)
                {
                    case spline_natural:
                        M.b[1] = 1;
                        M.b[n] = 1;
                        for(size_t j=0;j<ns;++j)
                        {
                            array<real_t> &y2 = *ppy2[j];
                            y2[1] = y2[n] = 0;
                        }
                        break;
                        
                    case spline_periodic:
                        throw exception("unexpected(spline_periodic)!!!");
                        
                        
                    case spline_tangent_left:
                        assert(ls_tab!=NULL);
                        M.b[1] = dx1/REAL(3.0);
                        M.c[1] = dx1/REAL(6.0);
                        for(size_t j=0;j<ns;++j)
                        {
                            const array<real_t> &y  = *ppy[j];
                            array<real_t>       &y2 = *ppy2[j];
                            const real_t dy1 = y[2] - y[1];
                            y2[1] = dy1/dx1 - ls_tab[j];
                            y2[n] = 0;
                        }
                        M.b[n] = 1;
                        break;
                        
                    case spline_tangent_right:
                        assert(rs_tab!=NULL);
                        M.b[n] = dxn / REAL(3.0);
                        M.a[n] = dxn / REAL(6.0);
                        for(size_t j=0;j<ns;++j)
                        {
                            const array<real_t> &y  = *ppy[j];
                            array<real_t>       &y2 = *ppy2[j];
                            const real_t        dyn = y[n] - y[nm];
                            y2[n]  = rs_tab[j] - dyn/dxn;
                            y2[1]  = 0;
                        }
                        M.b[1] = 1;
                        break;
                        
                    case spline_tangent_both:
                        assert(ls_tab!=NULL);
                        assert(rs_tab!=NULL);
                        M.b[1] = dx1/REAL(3.0);
                        M.c[1] = dx1/REAL(6.0);
                        M.b[n] = dxn/REAL(3.0);
                        M.a[n] = dxn/REAL(6.0);
                        for(size_t j=0;j<ns;++j)
                        {
                            const array<real_t> &y   = *ppy[j];
                            array<real_t>       &y2 = *ppy2[j];
                            const real_t dy1 = y[2] - y[1];
                            const real_t dyn = y[n] - y[nm];
                            y2[1] = dy1/dx1  - ls_tab[j];
                            y2[n] = rs_tab[j] - dyn/dxn;
                        }
                        break;
                        
                        
                }
                
                //==============================================================
                // solve all the y2
                //==============================================================
                for(size_t j=0;j<ns;++j)
                {
                    array<real_t> &y2 = *ppy2[j];
                    if( !M.solve(y2) )
                        throw exception("spline::compute(SINGULAR)");
                }
                
            }
            else
            {
                //==============================================================
                // periodic case
                //==============================================================
                const size_t     nm = n-1;
                cTriDiag<real_t> M(nm);
                
                //==============================================================
                // fill the core
                //==============================================================
                for(size_t i=2;i<nm;++i)
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
                        assert(ppy[j]);
                        assert(ppy2[j]);
                        const array<real_t> &y  = *ppy[j];  assert( n == y.size()  );
                        array<real_t>       &y2 = *ppy2[j]; assert( n == y2.size() );
                        const real_t yi = y[i];
                        y2[i]  = (y[ip]-yi)/dxp - (yi-y[im])/dxm;
                    }
                }
                
                //==============================================================
                // fill the sides
                //==============================================================
                const real_t dx1 = x[2] - x[1];
                const real_t dxn = x[n] - x[nm];
                M.b[1]  = (dx1+dxn)/3;
                M.c[1]  = dx1/6;
                M.c[nm] = dxn/6;
                
                const size_t nd = n-2;
                
                const real_t dxd = x[nm] - x[nd];
                M.b[nm] = (x[n] - x[nd])/3;
                M.a[nm] = dxd/6;
                M.a[1]  = dxn/6;
                
                for(size_t j=0;j<ns;++j)
                {
                    const array<real_t> &y  =  *ppy[j];
                    array<real_t>       &y2 = *ppy2[j];
                    const real_t yh  = REAL(0.5) * (y[1]+y[n]); //! regularize...
                    const real_t dy1 = y[2] - yh;
                    const real_t dyn = yh   - y[nm];
                    y2[1]  = (dy1/dx1 - dyn/dxn);
                    y2[nm] = (dyn/dxn - (y[nm]-y[nd])/dxd);
                }
                
                //==============================================================
                // solve and update y2
                //==============================================================
                for(size_t j=0;j<ns;++j)
                {
                    array<real_t> &y2 = *ppy2[j];
                    if( !M.solve(y2) )
                        throw exception("spline::compute(SINGULAR)");
                    y2[n] = y2[1];
                }
                
            }
            
        }
        
        template <>
        void spline<real_t>:: eval(real_t              *Y,
                                   const size_t         ns,
                                   real_t               X,
                                   const array<real_t> &x,
                                   const array<real_t> **ppy,
                                   const array<real_t> **ppy2,
                                   const real_t        *width)
        {
            assert(ns>0);
            assert(Y);
            assert(ppy);
            assert(ppy2);
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
                    for(size_t j=0;j<ns;++j) { assert(ppy[j]); Y[j] = (*ppy[j])[1]; }
                    return;
                }
                if( X >= xup )
                {
                    for(size_t j=0;j<ns;++j) { assert(ppy[j]); Y[j] = (*ppy[j])[n]; }
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
                assert(ppy[j]);
                assert(ppy2[j]);
                const array<real_t> &y  =  *ppy[j];
                const array<real_t> &y2 = *ppy2[j];
                Y[j] =  (a * y[klo]) + (b * y[khi]) + ( C*y2[klo]+D*y2[khi] ) * H;
            }
        }
        
        template<>
        void spline<real_t>:: derivs(array<real_t>       **ppy1,
                                     const array<real_t>  &x,
                                     const array<real_t> **ppy,
                                     const array<real_t> **ppy2,
                                     const size_t          ns )
        {
            assert(ppy1);
            assert(ppy);
            assert(ppy2);
            assert(ns>0);
            const size_t n = x.size();
            //------------------------------------------------------------------
            // core derivatives
            //------------------------------------------------------------------
            for(size_t i=1;i<n;++i)
            {
                const size_t ip = i+1;
                const real_t dx = x[ip]-x[i];
                for( size_t j=0;j<ns;++j)
                {
                    assert(ppy[j]);
                    assert(ppy1[j]);
                    assert(ppy2[j]);
                    const array<real_t> &y  =  *ppy[j];
                    const array<real_t> &y2 = *ppy2[j];
                    array<real_t>       &y1 = *ppy1[j];
                    const real_t dy = y[ip]-y[i];
                    y1[i] = dy/dx - dx * (y2[i]/REAL(3.0)+y2[ip]/REAL(6.0));
                }
            }
            
            //------------------------------------------------------------------
            // last derivatives
            //------------------------------------------------------------------
            const size_t nm = n-1;
            {
                const real_t dx = x[n] - x[nm];
                for(size_t j=0;j<ns;++j)
                {
                    const array<real_t> &y  =  *ppy[j];
                    const array<real_t> &y2 = *ppy2[j];
                    array<real_t>       &y1 = *ppy1[j];
                    const real_t dy = y[n] - y[nm];
                    y1[n] = dy/dx + dx * (y2[nm]/REAL(6.0)+y2[n]/REAL(3.0));
                }
            }
            
        }
        
        template <>
        spline<real_t>::  spline() throw() {}
        
        template <>
        spline<real_t>:: ~spline() throw() {}
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Spline 1D
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <>
        spline1D<real_t>:: ~spline1D() throw() {}
        
        static inline
        size_t __check_spline( const array<real_t> &X, const array<real_t> &Y)
        {
            const size_t n = X.size();
            if( X.size() != Y.size() ) throw exception("spline1D(X/Y mismatch)");
            if( n < 2) throw exception("spline1d(not enough data)");
            return n;
        }
        
        template <>
        spline1D<real_t>:: spline1D(spline_type          t,
                                    const array<real_t> &X,
                                    const array<real_t> &Y,
                                    const real_t        ls,
                                    const real_t        rs
                                    ) :
        x(X),
        y(Y),
        y2( __check_spline(X,Y), numeric<real_t>::zero ),
        w(X[X.size()]-X[1]),
        width( t==spline_periodic ? &w : 0),
        type(t)
        {
            spline<real_t>::compute( t, x, y, y2, ls, rs );
        }
        
        template <>
        real_t spline1D<real_t>:: get(const real_t X) const throw()
        {
            assert( x.size() == y.size() );
            assert( x.size() == y2.size() );
            assert( x.size() >= 2 );
            return spline<real_t>::eval(X, x, y, y2, width);
        }
        
        template <>
        void spline1D<real_t>:: recompute(const real_t ls, const real_t rs)
        {
            assert( x.size() == y.size() );
            assert( x.size() == y2.size() );
            assert( x.size() >= 2 );
            spline<real_t>::compute( type, x, y, y2, ls, rs );
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // Spline 2D
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        spline2D<real_t>:: ~spline2D() throw() {}
        
        
        static inline
        const array<real_t> & __check_t( const array<real_t> &t )
        {
            if( t.size() < 2 ) throw exception("spline2D(not enough data)");
            return t;
        }
        
        template <>
        spline2D<real_t>:: spline2D( const array<real_t> &user_t) :
        t( __check_t(user_t) ),
        n( t.size() ),
        P( 2, n ),
        Q( 2, n ),
        length(t[t.size()]-t[1]),
        width( 0 )
        {
            
        }
        
        template<>
        v2d<real_t> spline2D<real_t>:: operator()(real_t u) const throw()
        {
            return spline<real_t>::eval(u, t, P, Q, width);
        }
        
        
	}
    
}
