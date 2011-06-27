#ifndef YOCTO_CATALOG_INCLUDED
#define YOCTO_CATALOG_INCLUDED 1

#include "yocto/container/ordered.hpp"
#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/xarray.hpp"

namespace yocto
{
	
	namespace hidden
	{
		extern const char catalog_name[];
	}
	
	template <
	typename T,
	typename COMPARATOR = comparator<T>,
	typename ALLOCATOR  = memory::global::allocator
	>
	class catalog : public ordered<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		typedef core::xarray<mutable_type> xarr_t;
		typedef mutable_type              *type_ptr;
		
		explicit catalog() throw() : hmem_(), comp_(), xarr_() {}
		virtual ~catalog() throw() { _kill(); }
		
		inline catalog( size_t n, const as_capacity_t & ) : hmem_(), comp_(), xarr_(n,hmem_) {}
		inline catalog( const catalog &other ) : hmem_(), comp_(), xarr_()
		{
			catalog C( other.size() , as_capacity );
			other._copy_into(C);
			mswap( xarr_, C.xarr_ );
		}
		
		inline catalog & operator=( const catalog & other ) 
		{
			if( this != &other )
			{
				catalog C( other.size() , as_capacity );
				_copy_into(C);
				mswap( xarr_, C.xarr_ );
			}
			return *this;
		}
		
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw()     { return hidden::catalog_name;    }
		virtual size_t      size() const throw()     { return xarr_.slots.involved();  }
		virtual size_t      capacity() const throw() { return xarr_.slots.capacity();  }
		virtual void        free()    throw() { _free(); }
		virtual void        release() throw() { _kill(); }
		virtual void        reserve( size_t n ) 
		{
			if( n > 0 )
			{
				catalog C( this->capacity() + n , as_capacity );
				_copy_into(C);
				mswap( xarr_, C.xarr_ );
			}
		}
		
		
	private:
		ALLOCATOR  hmem_;
		COMPARATOR comp_;
		xarr_t     xarr_;
		
		inline void _free() throw() 
		{
			for( size_t i= this->size(); i > 0; )
			{
				type_ptr slot = xarr_.table[--i];
				destruct(slot);
				xarr_.slots.store( slot );
			}				
		}
		
		inline void _kill() throw() { _free(); xarr_.release_all( hmem_ ); }
		
		inline void _copy_into( catalog &C ) const
		{
			assert( C.size() == 0 ); assert( C.capacity() >= this->size() );
			const size_t num = this->size();
			for( size_t i=0; i < num; ++i )
			{
				const type_ptr src = xarr_.table[i];
				type_ptr       dst = C.xarr_.slots.query();
				try { new (dst) mutable_type( *src ); }
				catch(...) { C.xarr_.slots.store(dst); throw; }
				C.xarr_.table[i] = dst;
				assert( 0 == comp_( *src, *dst ) );
			}
			assert( C.size() == this->size() );
		}
	};
	
	
}


#endif
