#ifndef YOCTO_SHARED_PTR_INCLUDED
#define YOCTO_SHARED_PTR_INCLUDED 1


#include "yocto/type/args.hpp"
#include "yocto/ptr/ref.hpp"

namespace yocto
{

	template <typename T>
	class shared_ptr
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline shared_ptr( T *p ) : pointee_( (mutable_type*)p ), nref_(NULL)
		{
			assert( pointee_ );
			try { nref_ = core::create_ref(); } 
			catch(...) { delete p; throw; } 
			assert( nref_); assert( 1 == *nref_ );
		}

		inline ~shared_ptr() throw()
		{
			assert( nref_ ); assert( *nref_ );
			if( --(*nref_) <= 0 )
			{
				delete pointee_; pointee_ = NULL;
				core::delete_ref( nref_ ); assert( NULL == nref_ );
			}
		}
		
		inline shared_ptr( const shared_ptr &other ) throw() :
		pointee_( other.pointee_ ),
		nref_(    other.nref_    )
		{
			assert(nref_); assert( *nref_>0 );
			++(*nref_);
		}


		inline type & operator*()  throw()
		{
			assert(pointee_);
			return *pointee_;
		}
		
		inline const_type & operator*()  const throw()
		{
			assert(pointee_);
			return *pointee_;
		}
		
		inline type * operator->() throw() 
		{
			assert(pointee_);
			return pointee_;
		}
		
		inline const_type * operator->() const throw()
		{
			assert(pointee_);
			return pointee_;
		}
		
		inline type       * __get() throw()       { return pointee_; }
		inline const_type * __get() const throw() { return pointee_; }
        
        
	private:
		mutable_type *pointee_;
		size_t       *nref_;
		YOCTO_DISABLE_ASSIGN(shared_ptr);
	};
	
}

#endif
