#ifndef YOCTO_INTRUSIVE_PTR_INCLUDED
#define YOCTO_INTRUSIVE_PTR_INCLUDED 1


#include "yocto/type/key.hpp"

namespace yocto
{
	
	//! intrusive pointer
	/**
	 T must have:
	 - void   withhold() throw()
	 - bool   liberate() throw()
	 - size_t refcount() const throw()
	 */
	template <typename KEY,typename T>
	class intr_ptr
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		inline intr_ptr( T *p ) : pointee_( (mutable_type*)p )
		{
			assert( pointee_ );
			pointee_->withhold();
		}
		
		inline ~intr_ptr() throw()
		{
			assert(pointee_);
			if( pointee_->liberate() )
			{
				delete pointee_;
			}
		}
		
		inline intr_ptr( const intr_ptr &other ) throw() :
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
		
		inline const_key & key() const throw() { return pointee_->key(); }
		
        inline void swap_with( intr_ptr &other ) throw()
        {
            mutable_type *tmp = other.pointee_;
            other.pointee_    = pointee_;
            pointee_          = tmp;
        }
        
	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_ASSIGN(intr_ptr);
	};
	
	
		
	
}

#endif
