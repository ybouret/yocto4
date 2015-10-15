#include "yocto/math/kernel/cholesky.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto 
{
	
	namespace math 
	{
	
		template <>
		bool cholesky<real_t>:: build( matrix<real_t> &a, array<real_t> &diag ) throw()
		{
			assert( a.rows == diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			for( size_t i=1; i <= n; ++i )
			{
                matrix<real_t>::row &a_i = a[i];
				for( size_t j=i; j <=n; ++j )
				{
                    matrix<real_t>::row &a_j = a[j];
					real_t sum = a_i[j];
					for( size_t k=i-1;k>0;--k) 
						sum -= a_i[k] * a_j[k];
					
					if( i == j )
					{
						if( sum <= 0 )
							return false;
						diag[i] = Sqrt( sum );
					}
					else
					{
						a_j[i] = sum / diag[i];
					}
				}
			}
			return true;
		}
		
		template <>
		void cholesky<real_t>:: solve( const matrix<real_t> &a, const array<real_t> &diag, const array<real_t> &b, array<real_t> &x ) throw()
		{
			assert( a.rows == diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			assert( b.size() == n );
			assert( x.size() == n );
			
			for( size_t i=1; i <=n; ++i )
			{
				real_t sum = b[i];
				for( size_t k=i-1; k>0;--k ) 
					sum -= a[i][k] * x[k];
				x[i] = sum / diag[i];
			}
			
			for( size_t i=n; i>0; --i )
			{
				real_t sum = x[i];
				for( size_t k=i+1; k <=n; ++k )
					sum -= a[k][i] * x[k];
				x[i] = sum / diag[i];
			}
		}
        
        template <>
        void cholesky<real_t>:: Gram( matrix<real_t> &a, const matrix<real_t> &J)
        {
            const size_t n = a.rows;
            assert(a.is_square());
            assert(J.rows == a.rows);
            const size_t m = J.cols;
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=i;i>0;--j)
                {
                    real_t sum = 0;
                    for(size_t k=m;k>0;--k)
                    {
                        sum += J[i][k] * J[j][k];
                    }
                    a[i][j] = a[j][i] = sum;
                }
            }
        }

        template <>
		bool cholesky<real_t>:: inv_build( matrix<real_t> &a, array<real_t> &inv_diag ) throw()
		{
			assert( a.rows == inv_diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			for( size_t i=1; i <= n; ++i )
			{
                matrix<real_t>::row &a_i = a[i];
				for( size_t j=i; j <=n; ++j )
				{
                    matrix<real_t>::row &a_j = a[j];
					real_t sum = a_i[j];
					for( size_t k=i-1;k>0;--k)
						sum -= a_i[k] * a_j[k];
					
					if( i == j )
					{
						if( sum <= 0 )
							return false;
						inv_diag[i] = REAL(1.0) / Sqrt( sum );
					}
					else
					{
						a_j[i] = sum * inv_diag[i];
					}
				}
			}
			return true;
		}
        
        template <>
		void cholesky<real_t>:: inv_solve( const matrix<real_t> &a, const array<real_t> &inv_diag, const array<real_t> &b, array<real_t> &x ) throw()
		{
			assert( a.rows == inv_diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			assert( b.size() == n );
			assert( x.size() == n );
			
			for( size_t i=1; i <=n; ++i )
			{
				real_t sum = b[i];
				for( size_t k=i-1; k>0;--k )
					sum -= a[i][k] * x[k];
				x[i] = sum * inv_diag[i];
			}
			
			for( size_t i=n; i>0; --i )
			{
				real_t sum = x[i];
				for( size_t k=i+1; k <=n; ++k )
					sum -= a[k][i] * x[k];
				x[i] = sum * inv_diag[i];
			}
		}

        
			
	}
	
}
