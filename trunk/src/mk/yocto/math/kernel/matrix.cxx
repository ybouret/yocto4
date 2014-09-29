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
		data( NULL ),
		dlen( 0 ),
		buffer_( NULL ),
		buflen_(0),
        indx(NULL),
        scal(NULL)
		{
		}
		
		template <>
		void matrix<z_type>:: build()
		{
            static const size_t indx_size = max_of<size_t>( sizeof(size_t), sizeof(z_type) );
			if( size > 0 )
			{
                assert(dlen == size * sizeof(z_type) );
                //--------------------------------------------------------------
                // conditional allocation metrics
                //--------------------------------------------------------------
                const bool   allocExtra = (rows == cols);
                const size_t row_offset = 0;
                const size_t row_length = sizeof(row) * rows;
                const size_t itm_offset = YOCTO_MEMALIGN(row_length);
                const size_t itm_length = dlen;
                const size_t idx_offset = YOCTO_MEMALIGN(itm_length+itm_offset);
                const size_t idx_length = allocExtra ? rows * sizeof(indx_size) : 0;
                const size_t scl_offset = YOCTO_MEMALIGN(idx_length+idx_offset);
                const size_t scl_length = allocExtra ? rows * sizeof(z_type) : 0;
                const size_t tot_length = scl_offset + scl_length;
                
                //--------------------------------------------------------------
                // get buffer
                //--------------------------------------------------------------
                buflen_ = tot_length;
                buffer_ = memory::kind<memory::global>::acquire( buflen_ );
                
                uint8_t *p = (uint8_t *)buffer_;
                
                row_  = (row    *) &p[row_offset];
                data  = (z_type *) &p[itm_offset];
                if( allocExtra )
                {
                    const size_t  indx_bytes = rows * sizeof(size_t);
                    const size_t  copy_bytes = (idx_offset+indx_bytes) - itm_offset;
                    assert(copy_bytes>= size * sizeof(z_type) + rows * sizeof(size_t) );
                    
                    indx          = (size_t *)&p[idx_offset];
                    (size_t&)dlen = copy_bytes;  // copy item+indx
                    scal          = (z_type *)&p[scl_offset];
                }
                
                //--------------------------------------------------------------
				//-- construct rows
                //--------------------------------------------------------------
				--row_;
				new ( & row_[1] ) row( data - 1, cols );
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
		data( NULL ),
		dlen( size * sizeof(z_type) ),
		buffer_( NULL ),
		buflen_(0),
        indx(NULL),
        scal(NULL)
		{
			build();
		}
		
		template <>
		matrix<z_type>:: matrix( const matrix &M  ) :
		rows( M.rows ),
		cols( M.cols ),
		size( M.size ),
		row_( NULL   ),
		data( NULL   ),
		dlen( size * sizeof(z_type) ),
		buffer_( NULL   ),
		buflen_(0),
        indx(NULL),
        scal(NULL)
		{
			build();
            assert(dlen==M.dlen);
			memcpy( data, M.data, dlen );
		}
        
        template <>
		matrix<z_type>:: matrix( const matrix &M, const matrix_transpose_t &) :
		rows( M.cols ),
		cols( M.rows ),
		size( M.size ),
		row_( NULL   ),
		data( NULL   ),
		dlen( size*sizeof(z_type) ),
		buffer_(NULL),
		buflen_(0),
        indx(NULL),
        scal(NULL)
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
		void matrix<z_type>::minor_of( const matrix &M, size_t I, size_t J) throw()
        {
            assert(M.rows>1);
            assert(M.cols>1);
            assert(rows==M.rows-1);
            assert(cols==M.cols-1);
            
            matrix &self = *this;
            const size_t nr = M.rows;
            const size_t nc = M.cols;
            for(size_t i=1,ir=1;i<=nr;++i)
            {
                if(i!=I)
                {
                    for(size_t j=1,jr=1;j<=nc;++j)
                    {
                        if(j!=J)
                        {
                            self[ir][jr] = M[i][j];
                            ++jr;
                        }
                    }
                    ++ir;
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
			cswap( data, M.data );
			cswap_const( dlen, M.dlen );
			cswap( buffer_, M.buffer_ );
			cswap( buflen_, M.buflen_ );
            cswap( indx,    M.indx    );
            cswap( scal,    M.scal    );
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
					assert( size == M.size  );
					assert( dlen == M.dlen );
					memcpy( data, M.data, dlen );
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
				memset( data, 0, dlen );
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
			memset(data,0,dlen);
		}
		
		template <>
		void matrix<z_type>:: ld1() throw()
		{
			static const z_type __one(1);
			memset(data,0,dlen);
			for( size_t i = min_of<size_t>( rows, cols ); i>0; --i )
			{
				row_[i][i] = __one;
			}
		}
		
		template <>
		void matrix<z_type>:: assign( const matrix &other ) throw()
		{
			assert( this != &other );
			assert( cols  == other.cols );
			assert( rows  == other.rows );
			assert( size  == other.size );
			assert( dlen  == other.dlen );
			memcpy( data, other.data, dlen);
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
        
        template <>
        z_type &matrix<z_type>:: operator()(size_t i) throw()
        {
            assert(i<size);
            assert(data);
            return data[i];
        }
        
        template <>
        const z_type &matrix<z_type>:: operator()(size_t i) const throw()
        {
            assert(i<size);
            assert(data);
            return data[i];
        }

        template <>
        void * matrix<z_type>:: indx_addr() const throw()
        {
            return (void*)indx;
        }
        
        template <>
        void * matrix<z_type>:: scal_addr() const throw()
        {
            return (void*)scal;
        }

        
	}
}
