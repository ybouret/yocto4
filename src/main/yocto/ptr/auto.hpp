#ifndef YOCTO_AUTO_PTR_INCLUDED
#define YOCTO_AUTO_PTR_INCLUDED 1

#include "yocto/type/args.hpp"
namespace yocto
{
	
	template <typename T>
	class auto_ptr
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline auto_ptr()       throw() : pointee_( 0 ) {}		
		inline auto_ptr( T *p ) throw() : pointee_( (mutable_type *) p ) {}
		
		virtual ~auto_ptr() throw() { __kill(); }
		
		inline void reset( T *p ) throw()
		{
			assert( p==0 || p != pointee_ );
			__kill();
			pointee_ = (mutable_type *)p;
		}
        
        inline void release() throw() { reset(0); }
        
		
		inline auto_ptr & operator=( T *p ) throw()
		{
			if( p != pointee_ )
			{
				reset(p);
			}
			return *this;
		}
		
        inline void forget() throw() { pointee_ = 0; }
        
		inline T *  yield() throw()
		{
			T    *p  = pointee_;
			pointee_ = 0;
			return p;
		}

        inline void steal( auto_ptr &other ) throw()
        {
            __kill();
            pointee_       = other.pointee_;
            other.pointee_ = 0;
        }

        inline void swap_with( auto_ptr &other ) throw()
        {
            cswap(pointee_,other.pointee_);
        }

		inline type & operator*()  throw()
		{
			assert( is_valid() );
			return *pointee_;
		}
		
		inline const_type & operator*()  const throw()
		{
			assert( is_valid() );
			return *pointee_;
		}
		
		inline type * operator->() throw() 
		{
			assert( is_valid() );
			return pointee_;
		}
		
		inline const_type * operator->() const throw()
		{
			assert( is_valid() );
			return pointee_;
		}
		
		
		inline bool is_valid() const throw() { return 0 != pointee_; }
		
		inline type       * __get() throw()       { return pointee_; }
		inline const_type * __get() const throw() { return pointee_; }
		
	private:
		mutable_type *pointee_;
		YOCTO_DISABLE_COPY_AND_ASSIGN(auto_ptr);
		inline void __kill() throw()
		{
			if( pointee_ )
			{
				delete pointee_;
				pointee_ = 0;
			}
		}
		
	};
	
}

#endif
