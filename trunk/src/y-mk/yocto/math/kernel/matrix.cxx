#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/memory/global.hpp"
#include <cstring>
#include "yocto/core/offset.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/swap.hpp"

namespace yocto 
{
	namespace math
	{
		
		template <>
		matrix<z_type>::row:: row( z_type *item, size_t c ) throw() :
		cols( c     ),
		item_( item )
		{
			
		}
		
		template <>
		bool matrix<z_type>:: is_square() const throw()
		{
			return cols == rows;
		}
		
		template <>
		matrix<z_type>:: ~matrix() throw()
		{
			memory::kind<memory::global>::release( buffer_, buflen_ );
		}
		
		
		static inline
		size_t make_cols(const size_t &r, size_t c ) throw()
		{
			if( r > 0 )
			{
				return max_of<size_t>( c, 1 );
			}
			else
			{
				assert( r == 0 );
				if( c > 0 )
				{
					*(size_t *) &r = max_of<size_t>( r, 1 );
					return c;
				}
				else
				{
					assert( 0 == r && 0 == c );
					return 0;
				}
			}
		}
		
		
		template <>
		matrix<z_type>:: matrix() throw() :
		rows( 0 ),
		cols( 0 ),
		size( 0 ),
		row_( NULL ),
		ptr_( NULL ),
		len_( 0 ),
		buffer_( NULL ),
		buflen_(0)
		{
		}
		
		template <>
		void matrix<z_type>:: build()
		{
			if( size > 0 )
			{
				z_type **p = &ptr_;
				void   **q = (void **) p;
				//-- allocate memory
				memory::embed reg[] =
				{
					YOCTO_EMBED( row_, rows ),
					{ q, len_, 0 }
				};
				const size_t num = sizeof(reg)/sizeof(reg[0]);
				buflen_ = memory::embed::prepare( reg, num );
				buffer_ = memory::kind<memory::global>::acquire( buflen_ );
				memory::embed::dispatch( reg, num, buffer_ );
				
				//-- construct rows
				--row_;
				new ( & row_[1] ) row( ptr_ - 1, cols );
				for( size_t i = 2; i <= rows; ++i )
				{
					new ( & row_[i] ) row( row_[i-1].item_ + cols, cols );
				}
				
			}
		}
		
		template <>
		matrix<z_type>:: matrix( size_t r, size_t c ) :
		rows( r ),
		cols( make_cols( r,c ) ),
		size( rows * cols ),
		row_( NULL ),
		ptr_( NULL ),
		len_( size * sizeof(z_type) ),
		buffer_( NULL ),
		buflen_(0)
		{
			build();
		}
		
		template <>
		matrix<z_type>:: matrix( const matrix &M  ) :
		rows( M.rows ),
		cols( M.cols ),
		size( M.size ),
		row_( NULL   ),
		ptr_( NULL   ),
		len_( M.len_ ),
		buffer_( NULL   ),
		buflen_(0)
		{
			build();
			memcpy( ptr_, M.ptr_, len_ );
		}
		
		template <>
		void matrix<z_type>:: swap_with( matrix &M ) throw()
		{
			cswap_const( rows, M.rows );
			cswap_const( cols, M.cols );
			cswap_const( size, M.size );
			cswap( row_, M.row_ );
			cswap( ptr_, M.ptr_ );
			cswap_const( len_, M.len_ );
			cswap( buffer_, M.buffer_ );
			cswap( buflen_, M.buflen_ );
			
			//memswap( (void *) &rows, (void *) &M.rows, sizeof(matrix) - YOCTO_OFFSET_OF(matrix,rows) );
		}
		
		template <>
		void matrix<z_type>:: release() throw()
		{
			matrix    M;
			swap_with(M);
		}
		
		template <>
		matrix<z_type> & matrix<z_type>:: operator=( const matrix &M )
		{
			if( this != &M )
			{
				if( rows == M.rows && cols == M.cols )
				{
					assert( size == M.size );
					assert( len_ == M.len_ );
					memcpy( ptr_, M.ptr_, len_ );
				}
				else
				{
					matrix tmp( M );
					swap_with( tmp );
				}
			}
			return *this;
		}
		
		template <>
		void matrix<z_type>:: make( size_t r, size_t c )
		{
			if( r != rows || c != cols )
			{
				matrix tmp( r, c );
				swap_with(tmp);
			}
			else
			{
				memset( ptr_, 0, len_ );
			}
		}
		
		template <>
		matrix<z_type>::row &       matrix<z_type>::operator[]( size_t r )       throw()
		{
			assert( r >= 1 );
			assert( r <= rows );
			return row_[ r ];
		}
		
		template <>
		const matrix<z_type>::row &       matrix<z_type>::operator[]( size_t r ) const throw()
		{
			assert( r >= 1 );
			assert( r <= rows );
			return row_[ r ];
		}
		
		template <>
		z_type & matrix<z_type>::row:: operator[]( size_t c ) throw()
		{
			assert( item_     );
			assert( c >=1     );
			assert( c <= cols );
			return item_[c];
		}
		
		template <>
		const z_type & matrix<z_type>::row:: operator[]( size_t c ) const throw()
		{
			assert( item_     );
			assert( c >=1     );
			assert( c <= cols );
			return item_[c];
		}
		
		template <>
		void  matrix<z_type>:: swap_rows( size_t r1, size_t r2 ) throw()
		{
			assert( r1 >= 1 );
			assert( r2 >= 1 );
			
			assert( r1 <= rows );
			assert( r2 <= rows );
			
			z_type *p1 = row_[r1].item_;
			z_type *p2 = row_[r2].item_;
			YOCTO_LOOP(cols,cswap<z_type>( *(++p1), *(++p2) ) );
		}
		
		template <>
		void  matrix<z_type>:: swap_cols( size_t c1, size_t c2 ) throw()
		{
			assert( c1 >= 1 );
			assert( c2 >= 1 );
			
			assert( c1 <= cols );
			assert( c2 <= cols );
			
			z_type *p1 = row_[1].item_ + c1;
			z_type *p2 = row_[1].item_ + c2;
			
			YOCTO_LOOP(rows,cswap<z_type>( *p1, *p2 ); p1 += cols; p2 += cols );
		}
		
		
#define YOCTO_MATMUL(INDEX) ans += r_i[INDEX] * in[INDEX]
		
		template <>
		void matrix<z_type>::mul( array<z_type> &out, const array<z_type> &in ) const throw()
		{
			assert( out.size() == rows );
			assert( in.size()  == cols );
			assert( &out != &in );
			
			for( size_t i=rows; i>0; --i )
			{
				assert( cols > 0 );
				z_type      ans = 0;
				const row &r_i = row_[i];
				//for( size_t j=cols; j>0; --j ) ans += r_i[j] * in[j];
				YOCTO_LOOP_FUNC_(cols,YOCTO_MATMUL,1);
				out[i] = ans;
			}
			
		}
		
		template <>
		void matrix<z_type>:: ldz() throw()
		{
			memset(ptr_,0,len_);
		}
		
		template <>
		void matrix<z_type>:: ld1() throw()
		{
			memset(ptr_,0,len_);
			for( size_t i = min_of<size_t>( rows, cols ); i>0; --i )
			{
				row_[i][i] = 1;
			}
		}
		
		template <>
		void matrix<z_type>:: assign( const matrix &other ) throw()
		{
			assert( this != &other );
			assert( cols == other.cols );
			assert( rows == other.rows );
			assert( size == other.size );
			assert( len_ == other.len_ );
			memcpy( ptr_, other.ptr_, len_ );
		}
		
#if 0
		template <>
		z_type  * matrix<z_type>::       get_row_item( size_t r ) throw()
		{
			assert( r >= 1    );
			assert( r <= rows );
			return row_[r].item_;
		}
		
		template <>
		const z_type  * matrix<z_type>::       get_row_item( size_t r ) const  throw()
		{
			assert( r >= 1    );
			assert( r <= rows );
			return row_[r].item_;
		}
#endif
		
		template <>
		void matrix<z_type>:: transpose()
		{
			if( rows  > 0 )
			{
				const matrix<z_type> &A = *this;
				matrix<z_type>        M( cols, rows );
				for( size_t i=1; i <= rows; ++i )
				{
					for( size_t j=1; j <= cols; ++j )
					{
						M[j][i] = A[i][j];
					}
				}
				swap_with( M );
			}
			else {
				assert( 0 == cols );
			}
			
		}
		
	}
}
