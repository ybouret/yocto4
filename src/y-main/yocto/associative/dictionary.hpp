#ifndef YOCTO_DICTIONARY_INCLUDED
#define YOCTO_DICTIONARY_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/container/xarray.hpp"

namespace yocto
{
	namespace hidden
	{
		extern const char dictionary_name[];
	}
	
	template <
	typename KEY,
	typename T, 
	typename COMPARATOR = comparator<KEY>,
	typename ALLOCATOR  = memory::global::allocator
	>
	class dictionary : public container
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		class slot_t
		{
		public:
			const_key    key;
			type         data;
			inline  slot_t( param_key k, param_type d ) : key(k), data(d) {}
			inline ~slot_t() throw() {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(slot_t);
		};
		
		typedef core::xarray<slot_t> xarr_t;
		
		explicit dictionary() throw() : hmem_(), comp_(), xarr_() {}
		virtual ~dictionary() throw() { _kill(); }
		
		inline dictionary( size_t n, const as_capacity_t & ) : hmem_(), comp_(), xarr_(n,hmem_) {}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw()     { return hidden::dictionary_name; }
		virtual size_t      size() const throw()     { return xarr_.slots.involved();  }
		virtual size_t      capacity() const throw() { return xarr_.slots.capacity();  }
		virtual void        free()    throw() { _free(); }
		virtual void        release() throw() { _kill(); }
		virtual void        reserve( size_t n ) 
		{
			if( n > 0 )
			{
				dictionary D( this->capacity() + n , as_capacity );
				_copy_into(D);
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
				slot_t *slot = xarr_.table[--i];
				destruct(slot);
				xarr_.slots.store( slot );
			}				
		}
		
		inline void _kill() throw() { _free(); xarr_.release_all( hmem_ ); }
		inline void _copy_into( dictionary &D ) const
		{
			assert( D->size() == 0 ); assert( D->capacity() >= this->size() );
			const size_t num = this->size();
			for( size_t i=0; i < num; ++i )
			{
				const slot_t *src = xarr_.table[i];
				slot_t       *dst = D.xarr_.query();
				try { new (dst) slot_t( src->key, src->data ); }
				catch(...) { D.xarr_.store(dst); throw; }
				D.xarr_.table[i] = dst;
			}
			assert( D->size() == this->size() );
		}
	};
	
}

#endif

