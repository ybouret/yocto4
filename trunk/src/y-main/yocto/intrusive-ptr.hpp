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
	template <typename KEY,typename T>
	class intrusive_ptr
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		inline intrusive_ptr( T *p ) : pointee_( (mutable_type*)p )
		{
			assert( pointee_ );
			pointee_->withhold();
		}
		
		inline ~intrusive_ptr() throw()
		{
			assert(pointee_);
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
		
		inline const_key & key() const throw() { return pointee_->key(); }
		
	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_ASSIGN(intrusive_ptr);
	};
	
	
	class counted 
	{
	public:
		inline virtual ~counted() throw() { assert(nref_==0); }
		
		inline void withhold() throw() { ++nref_; }
		inline bool liberate() throw() { assert(nref_>0); return --nref_ <= 0; }
		
	protected:
		inline explicit counted() throw() : nref_(0) {}
		
	private:
		size_t nref_;
		YOCTO_DISABLE_COPY_AND_ASSIGN(counted);
	};
	
	
}

#endif
