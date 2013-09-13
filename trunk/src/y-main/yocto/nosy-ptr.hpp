#ifndef YOCTO_NOSY_PTR_INCLUDED
#define YOCTO_NOSY_PTR_INCLUDED 1


#include "yocto/type/sub-key.hpp"

namespace yocto
{
	
	//! intrusive pointer
	/**
	 T must have:
	 - void   withhold() throw()
	 - bool   liberate() throw()
	 - size_t refcount() const throw()
	 */
	template <typename KEY,typename SUBKEY,typename T>
	class nosy_ptr
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
        YOCTO_ARGUMENTS_DECL_KEY;
        YOCTO_ARGUMENTS_DECL_SUBKEY;
		
		inline nosy_ptr( T *p ) : pointee_( (mutable_type*)p )
		{
			assert( pointee_ );
			pointee_->withhold();
		}
		
		inline ~nosy_ptr() throw()
		{
			assert(pointee_);
			if( pointee_->liberate() )
			{
				delete pointee_;
			}
		}
		
		inline nosy_ptr( const nosy_ptr &other ) throw() :
		pointee_( other.pointee_ )
		{
			pointee_->withhold();
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
		
		inline const_key    & key()    const throw() { return pointee_->key();    }
        inline const_subkey & subkey() const throw() { return pointee_->subkey(); }

	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_ASSIGN(nosy_ptr);
	};
	
	
    
	
}

#endif
