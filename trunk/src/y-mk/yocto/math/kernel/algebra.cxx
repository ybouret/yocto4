#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

namespace yocto 
{
	
	namespace math 
	{
        
		template <>
		void algebra<z_type>:: set( array<z_type> &v, z_type a ) throw()
		{
			for( size_t i=v.size();i>0;--i) v[i] = a;
		}
		
		template <>
		void algebra<z_type>:: set( array<z_type> &v, const array<z_type> &u ) throw()
		{
			assert( v.size() == u.size() );
			for( size_t i=v.size();i>0;--i) v[i] = u[i];
		}
		
		template <>
		void algebra<z_type>:: add( array<z_type> &v, const array<z_type> &u ) throw()
		{
			assert( v.size() == u.size() );
			for( size_t i=v.size();i>0;--i) v[i] += u[i];
		}
		
        template <>
		void algebra<z_type>:: sub( array<z_type> &v, const array<z_type> &u ) throw()
		{
			assert( v.size() == u.size() );
			for( size_t i=v.size();i>0;--i) v[i] -= u[i];
		}
        
		template <>
		void algebra<z_type>:: muladd( array<z_type> &v, z_type a, const array<z_type> &u ) throw()
		{
			assert( v.size() == u.size() );
			for( size_t i=v.size();i>0;--i) v[i] += a * u[i];
		}
        
      
		
		template <>
		void algebra<z_type>:: mulset( array<z_type> &v, z_type a, const array<z_type> &u ) throw()
		{
			assert( v.size() == u.size() );
			for( size_t i=v.size();i>0;--i) v[i] = a * u[i];
		}
		
		
		
		template <>
		void algebra<z_type>:: mul( array<z_type> &v, const matrix<z_type> &M, const array<z_type> &u ) throw()
		{
			assert( v.size() == M.rows );
			assert( u.size() == M.cols );
			const size_t r = M.rows;
			const size_t c = M.cols;
			for( size_t i=r; i>0; --i)
			{
                const matrix<z_type>::row &M_i = M[i];
				z_type sum(0);
				for( size_t j=c; j>0; --j )
				{
					sum += M_i[j] * u[j];
				}
				v[i] = sum;
			}
		}
		
		template <>
		void algebra<z_type>:: muladd( array<z_type> &v, const matrix<z_type> &M, const array<z_type> &u ) throw()
		{
			assert( v.size() == M.rows );
			assert( u.size() == M.cols );
			const size_t r = M.rows;
			const size_t c = M.cols;
			for( size_t i=r; i>0; --i)
			{
                const matrix<z_type>::row &M_i = M[i];
				z_type sum(0);
				for( size_t j=c; j>0; --j )
				{
					sum += M_i[j] * u[j];
				}
				v[i] += sum;
			}
		}
		
		
		//----------------------------------------------------------------------
		
		template <>
		void algebra<z_type>:: mul_trn( array<z_type> &v, const matrix<z_type> &M, const array<z_type> &u ) throw()
		{
			assert( u.size() == M.rows );
			assert( v.size() == M.cols );
			const size_t r = M.rows;
			const size_t c = M.cols;
			for( size_t i=c; i>0; --i)
			{
				z_type sum(0);
				for( size_t j=r; j>0; --j )
				{
					sum += M[j][i] * u[j];
				}
				v[i] = sum;
			}
		}
		
		template <>
		void algebra<z_type>:: mul( matrix<z_type> &M, const matrix<z_type> &lhs, const matrix<z_type> &rhs) throw()
		{
			assert( M.rows == lhs.rows );
			assert( M.cols == rhs.cols );
			const size_t r = M.rows;
			const size_t c = M.cols;
			const size_t n = lhs.cols;
			assert( lhs.cols == rhs.rows );
			for( size_t i=r; i>0; --i )
			{
                const matrix<z_type>::row &lhs_i = lhs[i];
                matrix<z_type>::row       &M_i   = M[i];
				for( size_t j=c;j>0;--j)
				{
					z_type sum(0);
					for( size_t k=n; k>0; --k )
						sum += lhs_i[k] * rhs[k][j];
					M_i[j] = sum;
				}
			}
		}
		
		template <>
		void algebra<z_type>:: mul_ltrn( matrix<z_type> &M, const matrix<z_type> &lhs, const matrix<z_type> &rhs) throw()
		{
			assert( M.rows == lhs.cols );
			assert( M.cols == rhs.cols );
			const size_t r = M.rows;
			const size_t c = M.cols;
			const size_t n = lhs.rows;
			assert( lhs.rows == rhs.rows );
			for( size_t i=r; i>0; --i )
			{
                matrix<z_type>::row       &M_i   = M[i];
				for( size_t j=c;j>0;--j)
				{
					z_type sum(0);
					for( size_t k=n; k>0; --k )
						sum += lhs[k][i] * rhs[k][j];
					M_i[j] = sum;
				}
			}
		}
		
		
		template <>
		void algebra<z_type>:: mul_rtrn( matrix<z_type> &M, const matrix<z_type> &lhs, const matrix<z_type> &rhs) throw()
		{
			assert( M.rows == lhs.rows );
			assert( M.cols == rhs.rows );
			const size_t r = M.rows;
			const size_t c = M.cols;
			const size_t n = lhs.cols;
			assert( lhs.cols == rhs.cols );
			for( size_t i=r; i>0; --i )
			{
                const matrix<z_type>::row &lhs_i = lhs[i];
                matrix<z_type>::row       &M_i   = M[i];
				for( size_t j=c;j>0;--j)
				{
					z_type sum(0);
                    const matrix<z_type>::row &rhs_j = rhs[j];
					for( size_t k=n; k>0; --k )
						sum += lhs_i[k] * rhs_j[k];
					M_i[j] = sum;
				}
			}
		}
		
		template <>
		void algebra<z_type>:: gram_schmidt( matrix<z_type> &U, const matrix<z_type> &V, bool normalize)
		{
			assert(U.rows==V.rows);
			assert(U.cols==V.cols);
			const size_t r = U.rows;
			const size_t c = U.cols;
			U.assign(V);
			for( size_t k=1; k <= r; ++k )
			{
				const array<z_type> &Vk = V[k];
				for( size_t j=1; j < k; ++j )
				{
					const array<z_type> &Uj = U[j];
					z_type num(0);
					real_t den(0);
					for( size_t i=c;i>0;--i)
					{
						num += Uj[i] * Vk[i];
						const real_t u = Fabs(Uj[i]);
						den += u*u;
					}
					if( Fabs(den) <= 0 )
						throw exception( "Gram-Schmidt: singular matrix");
					const z_type   a(num/den);
					array<z_type> &Uk = U[k];
					for( size_t i=c;i>0;--i)
					{
						Uk[i] -= a * Uj[i];
					}
				}
			}
			if( normalize )
			{
				for( size_t k=r;k>0;--k)
				{
					array<z_type> &Uk = U[k];
					real_t         den(0);
					for( size_t i=c;i>0;--i)
					{
						const real_t u = Fabs(Uk[i]);
						den += u*u ;
					}
					den = Sqrt(den);
					for( size_t i=c;i>0;--i) Uk[i] /= den;
				}
			}
		}
		
	}
    
}
