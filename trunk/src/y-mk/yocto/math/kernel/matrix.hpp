#ifndef YOCTO_MATRIX_INCLUDED
#define YOCTO_MATRIX_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
	
	namespace math
	{
		
		template <typename T>
		class matrix : public object
			{
			public:
				class row : public array<T>
				{
				public:
					YOCTO_ARGUMENTS_DECL_T;
					const size_t cols;
					virtual type       & operator[]( size_t c ) throw();
					virtual const_type & operator[]( size_t c ) const throw();
					
					friend std::ostream & operator<<( std::ostream &os, const row &r )
					{
						for( size_t j=1; j <= r.cols; ++j ) os << r[j] << ' ';
						return os;
					}
					
					virtual const char *name() const throw() { return "matrix::row"; }
					virtual size_t size() const throw()      { return cols; }
					virtual size_t capacity() const throw()  { return cols; }
					virtual void   free() throw()            {}
					virtual void   release() throw()         {}
					virtual void   reserve(size_t)           {}

					
				private:
					row( T *item, size_t c ) throw();
					inline virtual ~row() throw() {}
					
					T *item_; //! offset-1 pointer
					YOCTO_DISABLE_COPY_AND_ASSIGN(row);
					friend class matrix<T>;
					virtual const_type * get_item() const throw() { return item_; }
				};
				
				explicit matrix() throw();
				explicit matrix( size_t r, size_t c );
				virtual ~matrix() throw();
				
				const size_t rows;   //!< #rows...
				const size_t cols;   //!< #cols...
				const size_t size;   //!< rows * cols
				
				matrix( const matrix & );
				matrix & operator=( const matrix & other );
				void assign( const matrix &other ) throw(); //!< same sizes
				
				void swap_with( matrix &M ) throw();
				void make( size_t r, size_t c );
				void release() throw();
				
				row &       operator[]( size_t r )       throw();
				const row & operator[]( size_t r ) const throw();
				
				void swap_rows( size_t r1, size_t r2 ) throw();
				void swap_cols( size_t c1, size_t c2 ) throw();
				
				
				bool is_square() const throw();
				
				friend std::ostream & operator<<( std::ostream &os, const matrix &M )
				{
					os << "[ ";
					if( M.rows > 0 )
					{
						for( size_t i=1; i < M.rows; ++i ) os << M[i] << "; ";
						os << M[ M.rows ];
					}
					os << "]";
					return os;
				}
				
				void mul( array<T> &out, const array<T> &in ) const throw();
				void ldz() throw();
				void ld1() throw(); //!< pseudo-id if not square
				
				//T *       get_row_item( size_t r ) throw();
				//const T * get_row_item( size_t r ) const throw();
				
				void transpose();
				
			private:
				row          *row_; //!< stride proxy
				T            *ptr_; //!< linear space
				const size_t  len_; //!< linear size for copy/set operations
				void         *buffer_; //!< dynamic workspace
				size_t        buflen_;
				void          build();
				
			};
		
	}
	
}

#endif
