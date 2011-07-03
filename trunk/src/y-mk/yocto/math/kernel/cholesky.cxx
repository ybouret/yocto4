#include "yocto/math/kernel/cholesky.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto 
{
	
	namespace math 
	{
	
		template <>
		bool cholesky<real_t>:: apply( matrix<real_t> &a, array<real_t> &diag ) throw()
		{
			assert( a.rows == diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			for( size_t i=1; i <= n; ++i )
			{
				for( size_t j=i; j <=n; ++j )
				{
					real_t sum = a[i][j];
					for( size_t k=i-1;k>0;--k) 
						sum -= a[i][k] * a[j][k];
					
					if( i == j )
					{
						if( sum <= 0.0 )
							return false;
						diag[i] = Sqrt( sum );
					}
					else
					{
						a[j][i] = sum / diag[i];
					}
				}
			}
			return true;
		}
		
		template <>
		void cholesky<real_t>:: solve( const matrix<real_t> &a, const array<real_t> &diag, const real_t *b, real_t *x ) throw()
		{
			assert( a.rows == diag.size() );
			assert( a.is_square() );
			const size_t n = a.rows;
			assert( !(b==NULL && n>0 ) );
			assert( !(x==NULL && n>0 ) );
			
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
			
	}
	
}
