#ifndef YOCTO_POINTER_ARC_INCLUDED
#define YOCTO_POINTER_ARC_INCLUDED 1

#include "yocto/type/args.hpp"

namespace yocto
{
    //! automatic references counting
    /**
     T must have:
     - void   withhold() throw()
     - bool   liberate() throw()
     - size_t refcount() const throw()
     */
    
    template <typename T>
    class arc_ptr
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        inline arc_ptr( T *p ) : pointee_( (mutable_type*)p )
		{
			assert( pointee_ );
			pointee_->withhold();
		}
		
		inline ~arc_ptr() throw()
		{
			assert(pointee_);
			if( pointee_->liberate() )
			{
				delete pointee_;
			}
		}
		
		inline arc_ptr( const arc_ptr &other ) throw() :
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
		
        inline void swap_with( arc_ptr &other ) throw()
        {
            mutable_type *tmp = other.pointee_;
            other.pointee_    = pointee_;
            pointee_          = tmp;
        }
        
		
	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_ASSIGN(arc_ptr);
        
    };
}

#endif
