#include "yocto/math/kernel/tridiag.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

#include <cmath>
#include <cerrno>

#include <iostream>

namespace yocto {
    
	namespace math {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // tridiag base
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <> tridiag_base<z_type>:: ~tridiag_base() throw() {}
        template <> tridiag_base<z_type>:: tridiag_base( size_t nxtra ) :
        arrays( 6 + nxtra ),
        a(  arrays.next_array() ),
        b(  arrays.next_array() ),
        c(  arrays.next_array() ),
        g(  arrays.next_array() ),
        xx( arrays.next_array() ),
        rr( arrays.next_array() )
        {
            
        }
        
        template <>
        void tridiag_base<z_type>:: make(size_t n)
        {
            arrays.prepare(n);
        }
        
        template <>
        size_t tridiag_base<z_type>:: size() const throw()
        {
            return arrays.common_size();
        }
        
        template <>
        void tridiag_base<z_type> :: output( std::ostream &os ) const
        {
            const tridiag_base<z_type> &self = *this;
            const size_t n = size();
            os << "[";
			for( size_t i=1; i <= n; ++i )
            {
				for( size_t j=1; j <= n; ++j )
                {
					os << " " << self(i,j);
				}
				if( i < n )
					os << " ;";
			}
			os << "];";
        }
        
        template <>
        void tridiag_base<z_type> :: apply( array<z_type> &v, const array<z_type> &u) const throw()
        {
            assert(v.size()==size());
            assert(u.size()==size());
            const size_t n = size();
            const tridiag_base<z_type> &self = *this;
            for(size_t i=n;i>0;--i)
            {
                z_type sum = numeric<z_type>::zero;
                for(size_t j=n;j>0;--j)
                    sum += self(i,j) * u[j];
                v[i] = sum;
            }
            
        }
        
        template <>
        void tridiag_base<z_type>:: apply( matrix<z_type>  &v, const matrix<z_type>  &u) const throw()
        {
            assert(size()==v.rows);
            assert(size()==u.rows);
            assert(u.cols==v.cols);
            const size_t n = size();
            for(size_t k=u.cols;k>0;--k)
            {
                for(size_t i=n;i>0;--i) g[i] = u[i][k];
                apply(xx,g);
                for(size_t i=n;i>0;--i) v[i][k] = xx[i];
            }
        }
        
        template <>
        bool tridiag_base<z_type>:: solve( array<z_type> &r) const throw()
        {
            assert(size()==r.size());
            for(size_t i=size();i>0;--i) xx[i] = r[i];
            return __solve(xx,r);
        }
        
        template <>
        bool tridiag_base<z_type>::solve( matrix<z_type> &x) const throw()
        {
            assert( size() == x.rows );
            assert(x.cols>0);
            const size_t n = size();
            for(size_t j=x.cols;j>0;--j)
            {
                for(size_t i=n;i>0;--i) rr[i]  = x[i][j];
                if( ! __solve(xx,rr) )     return false;
                for(size_t i=n;i>0;--i) x[i][j] = xx[i];
            }
            return true;
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // algorithms
        //
        ////////////////////////////////////////////////////////////////////////
        namespace
        {
            
            ////////////////////////////////////////////////////////////////////
            //
            // simple tridiag algorithm
            //
            ////////////////////////////////////////////////////////////////////
            static inline
            bool __tridiag(const array<z_type>  &a,
                           const array<z_type>  &b,
                           const array<z_type>  &c,
                           array<z_type>        &g,
                           array<z_type>        &u,
                           const array<z_type>  &r
                           ) throw()
            {
                const size_t n = a.size();
                assert(b.size()==n);
                assert(c.size()==n);
                assert(g.size()==n);
                assert(u.size()==n);
                assert(r.size()==n);
                
                z_type piv = b[1];
                if( Fabs( piv ) <= REAL_MIN )
                    return false;
                
                
                u[1] = r[1] / piv;
                
                for( size_t j=2, jm=1; j <= n; ++j,++jm )
                {
                    g[j] = c[jm] / piv;
                    piv  = b[j] - a[j] * g[j];
                    if( Fabs( piv ) <= REAL_MIN )
                        return false;
                    u[j] = (r[j] - a[j] * u[jm])/piv;
                }
                
                for( size_t j=n-1, jp=n;j>0;--j,--jp)
                {
                    assert(j+1==jp);
                    u[j] -= g[jp] * u[jp];
                }
                
                return true;
            }
            
            
            ////////////////////////////////////////////////////////////////////
            //
            // simple cyclic algorithm
            //
            ////////////////////////////////////////////////////////////////////
            static inline
            bool __cyclic(const array<z_type>  &a,
                          const array<z_type>  &b,
                          const array<z_type>  &c,
                          array<z_type>        &g,
                          array<z_type>        &u,
                          array<z_type>        &z,
                          array<z_type>        &bb,
                          array<z_type>        &x,
                          const array<z_type>  &r
                          ) throw()
            {
                const size_t n      =  a.size(); assert(n>=2);
                const z_type alpha  =  a[1];
                const z_type beta   =  c[n];
                const z_type gamma  = -b[1];
                
                //==============================================================
                // solve A.x = r
                //==============================================================
                bb[1]=b[1]-gamma;
                bb[n]=b[n]-(alpha*beta)/gamma;
                for(size_t i=2;i<n;i++)
                    bb[i]=b[i];
                if( ! __tridiag(a,bb,c,g,x,r) )
                    return false;
                
                //==============================================================
                // compute the vector u
                //==============================================================
                u[1]=gamma;
                u[n]=alpha;
                for(size_t i=2;i<n;i++)
                    u[i]=numeric<z_type>::zero;
                
                //==============================================================
                // solve A.z = u
                //==============================================================
                if(! __tridiag(a,bb,c,g,z,u) )
                    return false;
                
                //==============================================================
                // compute the factor
                //==============================================================
                const z_type fact=
                (x[1]+beta*x[n]/gamma)/
                (numeric<z_type>::one+z[1]+beta*z[n]/gamma);
                for(size_t i=n;i>0;--i)
                    x[i] -= fact*z[i];
                return true;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // simple tridiagonal matrix
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <>
        tridiag<z_type> :: ~tridiag() throw()
        {
            
        }
        
        template <>
        tridiag<z_type> :: tridiag( size_t n ) :
        tridiag_base<z_type>(0)
        {
            make(n);
        }
        
        template <>
        z_type tridiag<z_type>:: operator()(size_t i, size_t j) const throw()
        {
            assert(i>0);assert(i<=size());
            assert(j>0);assert(j<=size());
            
            const ptrdiff_t k = ptrdiff_t(i) - ptrdiff_t(j);
            switch( k )
            {
                case -1: return c[i];
                case  0: return b[i];
                case  1: return a[i];
                    
                default:
                    break;
            }
            return 0;
            
        }
        
        template <>
        bool tridiag<z_type>:: __solve( array<z_type> &x, const array<z_type> &r) const throw()
        {
            assert( size() == x.size() );
            assert( size() == r.size() );
            return __tridiag(a, b, c, g, x, r);
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // cyclic tridiagonal matrix
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        ctridiag<z_type> :: ~ctridiag() throw()
        {
            
        }
        
        template <>
        ctridiag<z_type> :: ctridiag( size_t n ) :
        tridiag_base<z_type>(3),
        u(  arrays.next_array() ),
        z(  arrays.next_array() ),
        bb( arrays.next_array() )
        {
            make(n);
        }
        
        template <>
        z_type ctridiag<z_type>:: operator()(size_t i, size_t j) const throw()
        {
            assert(i>0);assert(i<=size());
            assert(j>0);assert(j<=size());
            const size_t n = size();
            
            if(i==1&&j==n) return c[n];
            if(i==n&&j==1) return a[1];
            
            const ptrdiff_t k = ptrdiff_t(i) - ptrdiff_t(j);
            switch( k )
            {
                case -1: return c[i];
                case  0: return b[i];
                case  1: return a[i];
                    
                default:
                    break;
            }
            return 0;
            
        }
        
        template <>
        bool ctridiag<z_type>:: __solve( array<z_type> &x, const array<z_type> &r) const throw()
        {
            assert( size() == x.size() );
            assert( size() == r.size() );
            return __cyclic(a, b, c, g, u, z, bb, x, r);
        }
        
              
        
	}
    
    
}

