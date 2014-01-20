#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/memory/global.hpp"
#include <cstring>
#include "yocto/code/unroll.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
	namespace math
	{
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
		buflen_(0),
        indx_(NULL)
		{
		}
		
		template <>
		void matrix<z_type>:: build()
		{
			if( size > 0 )
			{
                //--------------------------------------------------------------
                // conditional allocation metrics
                //--------------------------------------------------------------
                const bool   alloc_indx = (rows == cols);
                const size_t row_offset = 0;
                const size_t row_length = sizeof(row) * rows;
                const size_t itm_offset = YOCTO_MEMALIGN(row_length);
                const size_t itm_length = len_;
                const size_t idx_offset = YOCTO_MEMALIGN(itm_length+itm_offset);
                const size_t idx_length = alloc_indx ? rows * sizeof(size_t) : 0;
                
                //--------------------------------------------------------------
                // get buffer
                //--------------------------------------------------------------
                buflen_ = idx_offset + idx_length;
                buffer_ = memory::kind<memory::global>::acquire( buflen_ );
                
                uint8_t *p = (uint8_t *)buffer_;
                
                row_ = (row    *) &p[row_offset];
                ptr_ = (z_type *) &p[itm_offset];
                if( alloc_indx )
                {
                    indx_ = (size_t *)&p[idx_offset];
                    --indx_;
                }
                
                //--------------------------------------------------------------
				//-- construct rows
                //--------------------------------------------------------------
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
		buflen_(0),
        indx_(NULL)
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
		buflen_(0),
        indx_(NULL)
		{
			build();
			memcpy( ptr_, M.ptr_, len_ );
		}
        
        template <>
		matrix<z_type>:: matrix( const matrix &M, const matrix_transpose_t &) :
		rows( M.cols ),
		cols( M.rows ),
		size( M.size ),
		row_( NULL   ),
		ptr_( NULL   ),
		len_( M.len_ ),
		buffer_( NULL   ),
		buflen_(0),
        indx_(NULL)
		{
			build();
            matrix<z_type> &self = *this;
            for( size_t i=1; i <= rows; ++i )
            {
                for( size_t j=1; j <= cols; ++j )
                {
                    self[i][j] = M[j][i];
                }
            }
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
            cswap( indx_,   M.indx_   );
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
		
        
		template <>
		void matrix<z_type>:: ldz() throw()
		{
			memset(ptr_,0,len_);
		}
		
		template <>
		void matrix<z_type>:: ld1() throw()
		{
			static const z_type __one(1);
			memset(ptr_,0,len_);
			for( size_t i = min_of<size_t>( rows, cols ); i>0; --i )
			{
				row_[i][i] = __one;
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
		
        template <>
        void matrix<z_type>:: diag( const array<z_type> &w )
        {
            const size_t n = w.size();
            make(n,n);
            for(size_t i=n;i>0;--i) (*this)[i][i] = w[i];
        }
        
		
		template <>
		void matrix<z_type>:: transpose()
		{
            matrix<z_type> M( *this, matrix_transpose);
            swap_with(M);
		}
        
        template <>
        void matrix<z_type>:: swap_both( size_t u, size_t v ) throw()
        {
            assert(is_square());
            swap_cols(u,v);
            swap_rows(u,v);
        }

        
		
	}
}
