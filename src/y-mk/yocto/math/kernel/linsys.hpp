#ifndef YOCTO_LINSYS_INCLUDED
#define YOCTO_LINSYS_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto 
{
	
	namespace math 
	{
		
		template <typename T>
		class linsys : public array<T>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef typename real_of<T>::type real_type;
			typedef size_t                    indx_type;
			
			explicit linsys() throw();
			explicit linsys( size_t n );
			virtual ~linsys() throw();
			
			virtual const char *name() const throw();
			virtual size_t      size() const throw();
			virtual size_t      capacity() const throw();
			virtual void        free() throw();
			virtual void        release() throw();
			virtual void        reserve( size_t n );
			
			void swap_with( linsys &L ) throw();
			void ensure( size_t n ); //!< ensure size >= n
			
			bool LU( matrix<T> &M ) throw(); //!< LU decomposition
			
			
			void operator()( const matrix<T> &M, array<T>  &b ) const throw();
			void operator()( const matrix<T> &M, matrix<T> &Q ) const throw();
			void operator()( const matrix<T> &M ) throw(); //!< using internal item !
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(linsys);
			const size_t       size_; //!< maximum system size
			mutable T         *item_; //!< workspace
			mutable real_type *scal_; //!< partial workspace
			size_t            *indx_; //!< row exchange info
			void              *buffer_;  //!< allocated memory
			size_t             buflen_;  //!< allocated bytes
			
			virtual const_type *get_item() const throw();
		public:
			bool       dneg; //!< if determinant is negative
			
		};
		
		
		
	}
}

#endif
