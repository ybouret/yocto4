#ifndef YOCTO_INTRUSIVE_PTR_INCLUDED
#define YOCTO_INTRUSIVE_PTR_INCLUDED 1


#include "yocto/type-traits.hpp"

namespace yocto
{
		
	//! intrusive pointer
	/**
		T must have:
		- void   withhold() throw()
		- bool   liberate() throw()
		- size_t refcount() const throw()
	 */
	template <typename T>
	class intrusive_ptr
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline intrusive_ptr( T *p ) : pointee_( (mutable_type*)p )\
		{
			assert( pointee_ );
			pointee_->withhold();
		}
		
		inline ~intrusive_ptr() throw()
		{
			if( pointee_->liberate() )
			{
				delete pointee_;
			}
		}
		
		inline intrusive_ptr( const intrusive_ptr &other ) throw() :
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
		
	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_ASSIGN(intrusive_ptr);
	};
	
	
}

#endif
