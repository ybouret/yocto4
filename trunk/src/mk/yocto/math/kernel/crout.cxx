#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        crout<z_type>:: crout() throw() :
        scal_type(),
        dneg(false)
        {
        }
        
        template <>
        crout<z_type>:: crout(size_t n) :
        scal_type(n,numeric<real_type>::zero),
        dneg(false)
        {
        }
        
        template <>
        crout<z_type>:: ~crout() throw()
        {
        }
        
        
        template <>
        bool crout<z_type>:: build( matrix<z_type> &a ) throw()
        {
            
            static const z_type z1(1);
            assert( a.cols   > 0   );
			assert( a.is_square()  );
            assert( a.indx != NULL );
			const size_t      n    = a.rows;
			array<real_t>    &scal = *this;   assert( this->size() >= a.rows );
            lw_array<size_t>  indx( a.indx, n);
            
			//------------------------------------------------------------------
			//
			// initialize implicit pivots
			//
			//------------------------------------------------------------------
            dneg = false;
			for( size_t i=n;i>0;--i)
			{
				const matrix<z_type>::row & a_i = a[i];
				real_t                      piv = 0;
				
				for( size_t j=n;j>0;--j)
				{
					const real_t tmp = Fabs( a_i[j] );
					if ( tmp > piv )
                    {
						piv = tmp;
                    }
                }
				if( piv <= REAL_MIN )
				{
					//std::cerr << "-- LU failure level-1" << std::endl;
					return false;
				}
				scal[i] = REAL(1.0)/piv;
			}
			
			
			//------------------------------------------------------------------
			// Crout's algorithm
			//------------------------------------------------------------------
			for(size_t j=1;j<=n;++j)
			{
				for(size_t i=1;i<j;++i)
				{
					matrix<z_type>::row &a_i = a[i];
					z_type               sum = a_i[j];
					for(size_t k=1;k<i;++k)
                    {
						sum -= a_i[k]*a[k][j];
                    }
                    a_i[j]=sum;
                }
				
				real_t piv  = 0;
				size_t imax = j;
				for( size_t i=j;i<=n;i++)
				{
					matrix<z_type>::row &a_i = a[i];
					
                    z_type sum=a_i[j];
					for(size_t k=1;k<j;++k)
                    {
						sum -= a_i[k]*a[k][j];
                    }
                    a_i[j]=sum;
                    
                    const real_t tmp = scal[i]*Fabs(sum);
                    if( tmp >= piv )
                    {
                        piv  = tmp;
                        imax = i;
                    }
				}
				
				//-- TODO: check ?
				//assert( piv > 0 );
				assert( imax> 0 );
				if (j != imax)
				{
					a.swap_rows( j, imax );
					dneg = !dneg;
					scal[imax]=scal[j];
				}
				
				indx[j]=imax;
				
				if( Fabs(a[j][j]) <= REAL_MIN )
				{
					//std::cerr << "-- LU failure level-2" << std::endl;
					return false;
				}
				
				if (j != n)
				{
					const z_type fac = z1/(a[j][j]);
					for(size_t i=j+1;i<=n;++i)
                    {
						a[i][j] *= fac;
                    }
                }
			}
			
#if !defined(NDEBUG)
			for( size_t i=1; i <= n; ++i )
			{
				assert( indx[i] >  0 );
				assert( indx[i] <= n );
			}
#endif
            return true;
            
            
        }
        
        template <>
        void crout<z_type>:: solve(const matrix<z_type>     &a,
                                   array<z_type>            &b) throw()
        {
            assert( a.cols   > 0   );
			assert( a.is_square()  );
            assert( a.indx );
            assert( b.size()   == a.rows );
            
            const size_t     n  = a.rows;
            size_t           ii = 0;
            const lw_array<size_t> indx( a.indx, n );
            
			for(size_t i=1;i<=n;++i)
			{
				const size_t  ip  = indx[i]; assert(ip>0);
				z_type        sum = b[ip];
				
				b[ip]=b[i];
				if(ii)
				{
					for( size_t j=ii; j<= i-1; ++j)
                    {
						sum -= a[i][j]*b[j];
                    }
                }
				else
				{
					if( Fabs(sum) > 0 )
                    {
						ii=i;
                    }
                }
				b[i]=sum;
			}
			
			for(size_t i=n;i>0; --i)
			{
				z_type sum=b[i];
				for(size_t j=i+1;j<=n;++j)
                {
					sum -= a[i][j]*b[j];
                }
                b[i]=sum/a[i][i];
            }
            
        }
        
        
        template <>
        void crout<z_type>:: solve(const matrix<z_type>     &M,
                                   matrix<z_type>           &Q)  throw()
        {
            assert( M.cols   > 0     );
			assert( M.is_square()    );
			assert( M.indx );
			assert( Q.rows == M.rows );
			
			const size_t           n = M.rows;
			const size_t           m = Q.cols;
            const lw_array<size_t> indx( M.indx, n );
            
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=1;i<=n;++i)
				{
					const size_t ip  = indx[i];
					z_type       sum = Q[ip][k];
					
					Q[ip][k]=Q[i][k];
					
					for( size_t j=1; j<i;j++)
                    {
                        sum -= M[i][j]*Q[j][k];
                    }
                    
                    Q[i][k]=sum;
                }
			}
			
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=n;i>0;--i)
				{
					z_type                sum = Q[i][k];
					for(size_t j=i+1;j<=n;j++)
                    {
						sum -= M[i][j]*Q[j][k];
                    }
                    Q[i][k]=sum/M[i][i];
                }
			}
            
        }
        
        template <>
        bool crout<z_type>:: inverse(matrix<z_type> &a)
        {
            matrix<z_type> b(a);
            if( ! build(b) ) return false;
            a.ld1();
            crout<z_type>::solve(b,a);
            return true;
        }
        
        template <>
        bool crout<z_type>:: pseudo_inverse(matrix<z_type>       &M,
                                            const matrix<z_type> &P)
        {
            assert(P.rows>0);
            assert(P.cols>=P.rows);
            assert(M.cols==P.rows);
            assert(M.rows==P.cols);
            
            const size_t n = P.rows;
            const size_t m = P.cols;
            matrix<z_type> P2(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    z_type sum(0);
                    for(size_t k=m;k>0;--k)
                    {
                        sum += P[i][k] * P[j][k];
                    }
                    P2[i][j] = sum;
                }
            }
            
            if( !inverse(P2) )
                return false;
            
            for(size_t i=m;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    z_type sum(0);
                    for(size_t k=n;k>0;--k)
                    {
                        sum += P[k][i] * P2[k][j];
                    }
                    M[i][j] = sum;
                }
            }
            
            return true;
        }
        
        
        
    }
}
