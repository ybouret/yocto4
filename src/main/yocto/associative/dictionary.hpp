#ifndef YOCTO_DICTIONARY_INCLUDED
#define YOCTO_DICTIONARY_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/xarray.hpp"
#include "yocto/core/locate.hpp"
#include "yocto/container/iter-dict.hpp"

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
	class dictionary : public associative<KEY,T>
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
		typedef slot_t *             slot_ptr;
		
		explicit dictionary() throw() : hmem_(), comp_(), xarr_() {}
		virtual ~dictionary() throw() { _kill(); }
		
		inline dictionary( size_t n, const as_capacity_t & ) : hmem_(), comp_(), xarr_(n,hmem_) {}
		inline dictionary( const dictionary &other ) : hmem_(), comp_(), xarr_()
		{
			dictionary D( other.size() , as_capacity );
			other._copy_into(D);
			bswap( xarr_, D.xarr_ );
		}
		
		inline dictionary & operator=( const dictionary & other ) 
		{
			if( this != &other )
			{
				dictionary D( other.size() , as_capacity );
				_copy_into(D);
				bswap( xarr_, D.xarr_ );
			}
			return *this;
		}
		
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
				bswap( xarr_, D.xarr_ );
			}
		}
		
		//======================================================================
		// associative interface
		//======================================================================
		virtual bool remove( param_key key ) throw()
		{
			size_t indx = 0;
			if( core::locate<key_type,slot_ptr,const dictionary&>( (key_type *)&key, xarr_.table, size(), indx, *this ) )
			{
				slot_ptr *addr = &xarr_.table[indx];
				slot_t   *slot = *addr;
				destruct(slot);
				xarr_.slots.store(slot);
				memmove( addr, addr+1, (size()-indx) * sizeof(slot_ptr) );
				return true;
			}
			else
				return false;
		}
		
		virtual bool insert( param_key key, param_type data )
		{
			size_t indx = 0;
			if( core::locate<key_type,slot_ptr,const dictionary&>( (key_type *)&key, xarr_.table, size(), indx, *this ) )
			{
				return false;
			}
			else
			{
				if( xarr_.slots.available() > 0 )
				{
					_insert( key, data, indx );
				}
				else 
				{
					dictionary D( container::next_capacity( this->capacity() ), as_capacity );
					_copy_into(D); assert( D.xarr_.slots.available() > 0 );
					D._insert(key,data,indx);
					bswap( xarr_, D.xarr_ );
				}
				return true;
			}
		}
		
		//======================================================================
		// iterators
		//======================================================================
		typedef iterating::dict<type,slot_t,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator( xarr_.table  );      }
		inline iterator end()   throw() { return iterator( xarr_.table+size()); }
		
		typedef iterating::dict<const_type,slot_t,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return const_iterator( xarr_.table  );      }
		inline const_iterator end()   const throw() { return const_iterator( xarr_.table+size()); }
		
		
		
	private:
		ALLOCATOR  hmem_;
		COMPARATOR comp_;
		xarr_t     xarr_;
		
		
		virtual const_type *lookup( param_key key ) const throw()
		{
			size_t indx = 0;
			if( core::locate<key_type,slot_t*,const dictionary&>( (key_type *)&key, (slot_t**)xarr_.table, this->size(), indx, *this ) )
			{
				return  & (xarr_.table[indx]->data);
			}
			else
				return NULL;
		}
		
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
			assert( D.size() == 0 ); assert( D.capacity() >= this->size() );
			const size_t num = this->size();
			for( size_t i=0; i < num; ++i )
			{
				const slot_t *src = xarr_.table[i];
				slot_t       *dst = D.xarr_.slots.query();
				try { new (dst) slot_t( src->key, src->data ); }
				catch(...) { D.xarr_.slots.store(dst); throw; }
				D.xarr_.table[i] = dst;
				assert( 0 == comp_( src->key, dst->key ) );
			}
			assert( D.size() == this->size() );
		}
		
		inline void _insert( param_key key, param_type data, const size_t indx )
		{
			const size_t num = size();
			assert( xarr_.slots.available() > 0 );
			slot_t *dst = xarr_.slots.query();
			try { new (dst) slot_t( key, data); }
			catch(...) { xarr_.slots.store(dst); throw; }
			core::insert<slot_t*>( &dst, xarr_.table, num, indx );
		}
		
		
		
	public:
		inline int operator()( param_key lhs, slot_ptr slot ) const throw()
		{
			return comp_(lhs,slot->key);
		}
	};
	
}

#endif

