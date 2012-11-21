#include "yocto/math/kernel/lu.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
    namespace math
    {
        
        template <>
        lu<z_type>:: lu() throw() :
        indx(),
        scal(),
        buffer_(0),
        buflen_(0),
        maxi_(0),
        dneg(false)
        {
            
        }
        
        
        
        template <>
        void lu<z_type>:: release() throw()
        {
            memory::kind<memory::global>::release( buffer_, buflen_ );
            maxi_ = 0;
            indx.reset(0,0);
            scal.reset(0,0);
        }
        
        template <>
        lu<z_type>:: ~lu() throw()
        {
            release();
        }
        
        template <>
        void lu<z_type>:: ensure( size_t n )
        {
            if(n>maxi_)
            {
                release();
                const size_t indx_size = n * sizeof(indx_type);
                const size_t scal_size = n * sizeof(real_type);
                buflen_ = indx_size + scal_size;
                buffer_ = memory::kind<memory::global>::acquire(buflen_);
                maxi_   = n;
            }
        }
        
        template <>
        lu<z_type>:: lu(size_t n) :
        indx(),
        scal(),
        buffer_(0),
        buflen_(0),
        maxi_(0),
        dneg(false)
        {
            ensure(n);
        }
        
        template <>
        void lu<z_type>:: link( size_t n ) throw()
        {
            assert(n>0);
            assert(n<=maxi_);
            size_t *p = static_cast<size_t*>(buffer_);
            indx.reset(p,n);
            p += n;
            scal.reset( (real_type*)p,n);
        }
        
        template <>
        bool lu<z_type>::build(matrix<z_type> &a) throw()
        {
            static const z_type z1(1);
            assert( a.cols   > 0   );
			assert( a.is_square()  );
			const size_t n = a.rows;
            link(n);
			assert( indx.size() == n );
            assert( scal.size() == n );
            
			//------------------------------------------------------------------
			//
			// initialize implicit pivots
			//
			//------------------------------------------------------------------
			for( size_t i=n;i>0;--i)
			{
				const matrix<z_type>::row & a_i = a[i];
				real_t                      piv = 0;
				
				for( size_t j=n;j>0;--j)
				{
					const real_t tmp = Fabs( a_i[j] );
					if ( tmp > piv )
						piv = tmp;
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
						sum -= a_i[k]*a[k][j];
					a_i[j]=sum;
				}
				
				real_t piv  = 0;
				size_t imax = j;
				for( size_t i=j;i<=n;i++)
				{
					matrix<z_type>::row &a_i = a[i];
					
                    z_type sum=a_i[j];
					for(size_t k=1;k<j;++k)
						sum -= a_i[k]*a[k][j];
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
						a[i][j] *= fac;
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
        void lu<z_type>:: solve(const matrix<z_type>     &a,
                                array<z_type>            &b) const throw()
        {
            assert( a.cols   > 0   );
			assert( a.is_square()  );
			assert( indx.size() == a.rows );
            assert( b.size() == a.rows );
            
            const size_t n = a.rows;
            size_t       ii = 0;
            
			for(size_t i=1;i<=n;++i)
			{
				const size_t  ip  = indx[i]; assert(ip>0);
				z_type        sum = b[ip];
				
				b[ip]=b[i];
				if(ii)
				{
					for( size_t j=ii; j<= i-1; ++j)
						sum -= a[i][j]*b[j];
				}
				else
				{
					if( Fabs(sum) > 0 )
						ii=i;
				}
				b[i]=sum;
			}
			
			for(size_t i=n;i>0; --i)
			{
				z_type sum=b[i];
				for(size_t j=i+1;j<=n;++j)
					sum -= a[i][j]*b[j];
				b[i]=sum/a[i][i];
			}
            
        }
        
        template <>
        void lu<z_type>:: solve(const matrix<z_type>     &M,
                                matrix<z_type>           &Q) const throw()
        {
            assert( M.cols   > 0     );
			assert( M.is_square()    );
			assert( M.rows == indx.size() );
			assert( Q.rows == M.rows );
			
			const size_t n = M.rows;
			const size_t m = Q.cols;
			
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=1;i<=n;++i)
				{
					const size_t ip           = indx[i];
					z_type                sum = Q[ip][k];
					
					Q[ip][k]=Q[i][k];
					
					for( size_t j=1; j<i;j++)
						sum -= M[i][j]*Q[j][k];
					
					Q[i][k]=sum;
				}
			}
			
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=n;i>0;--i)
				{
					z_type                sum = Q[i][k];
					for(size_t j=i+1;j<=n;j++)
						sum -= M[i][j]*Q[j][k];
					Q[i][k]=sum/M[i][i];
				}
			}
            
        }
        
    }
}
