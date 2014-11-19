#ifndef YOCTO_MATRIX_INCLUDED
#define YOCTO_MATRIX_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
    
	namespace math
	{
		
    
        enum matrix_ctor
        {
            matrix_ctor_normal,
            matrix_ctor_transpose
        };
        
        typedef int2type<matrix_ctor_normal>    matrix_normal_t;
        typedef int2type<matrix_ctor_transpose> matrix_transpose_t;
        
        extern const matrix_normal_t    matrix_normal;
        extern const matrix_transpose_t matrix_transpose;
        
        
        
        
		template <typename T>
		class matrix : public object
			{
			public:
                YOCTO_ARGUMENTS_DECL_T;

				class row : public array<T>
				{
				public:
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
				
				matrix( const matrix & ); //!< copy
                matrix( const matrix &, const matrix_transpose_t &);
				matrix & operator=( const matrix & other ); //!< assign
				void assign( const matrix &other ) throw(); //!< MUST have same sizes
				void diag( const array<T> &w ); //!< make a diagonal matrix
                
                
				void swap_with( matrix &M ) throw();
				void make( size_t r, size_t c ); //!< set to zero
				void release() throw();
				
				row &       operator[]( size_t r )       throw();
				const row & operator[]( size_t r ) const throw();
				
				void swap_rows( size_t r1, size_t r2 ) throw();
				void swap_cols( size_t c1, size_t c2 ) throw();
				void swap_both( size_t u, size_t v ) throw();
                
				
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
				
				void ldz() throw();
				void ld1() throw(); //!< pseudo-id if not square
				void transpose();
				
                T       & operator()(size_t i) throw();       //!< in [0:size-1]
                const T & operator()(size_t i) const throw(); //!< in [0:size-1]
                
                void minor_of( const matrix &M, size_t I, size_t J) throw();
                
                template <typename U>
                inline void minor_of(const matrix<U> &M,
                                     const size_t     I,
                                     const size_t     J) throw()
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
                                    self[ir][jr] = T(M[i][j]);
                                    ++jr;
                                }
                            }
                            ++ir;
                        }
                    }

                }
                
			private:
				row          *row_;    //!< stride proxy
				
                T            *data;    //!< linear space
				const size_t  dlen;    //!< linear size for copy/set operations
                
				void         *buffer_; //!< dynamic workspace
				size_t        buflen_; //!< dynamic workspace size
                
				void          build();
                
            public:
                size_t       *indx;   //!< not NULL for square matrices, enough room for (T *) !!
                mutable T    *scal;   //!< not NULL for square matrices
                
                template <typename U>
                inline U * get_aux() const throw()
                {
                    assert(sizeof(U)<=sizeof(T));
                    return static_cast<U*>( indx_addr() );
                }
                
                template <typename U>
                inline U *get_aux2() const throw()
                {
                    assert(sizeof(U)<=sizeof(T));
                    return static_cast<U*>( scal_addr() );
                }
                
            private:
                void *indx_addr() const throw();
                void *scal_addr() const throw();
			};
		
	}
	
}

#endif
