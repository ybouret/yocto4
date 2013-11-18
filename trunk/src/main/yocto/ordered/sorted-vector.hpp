#ifndef YOCTO_SORTED_VECTOR_INCLUDED
#define YOCTO_SORTED_VECTOR_INCLUDED 1

#include "yocto/container/ordered.hpp"
#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/locate.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/bitwise.hpp"

#include <iostream>

namespace yocto
{
	
	namespace hidden
	{
		extern const char sorted_vector_name[];
	}
	
	//! a sorted vector of bitwise ops supporting object
	template <
	typename T,
	typename COMPARATOR = comparator<T>,
	typename ALLOCATOR  = memory::global::allocator
	>
	class sorted_vector : public ordered<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		explicit sorted_vector() throw() :
		hmem_(), size_(0), maxi_(0), addr_(NULL), item_(addr_-1), comp_()
		{
		}
		
		explicit sorted_vector( size_t n, const as_capacity_t & ) :
		hmem_(), size_(0), maxi_(n), 
		addr_(static_cast<memory::allocator&>(hmem_).acquire_as<T>(maxi_)),
		item_(addr_-1),
		comp_()
		{
		}
		
		sorted_vector( const sorted_vector &other ) :
		hmem_(),
		size_(0),
		maxi_(other.size_),
		addr_(static_cast<memory::allocator&>(hmem_).acquire_as<T>(maxi_)),
		item_(addr_-1),
		comp_()
		{
			other.duplicate_into( *this );
		}
		
		virtual ~sorted_vector() throw()
		{
			_release();
		}
		
		virtual const char *name() const throw()      { return hidden::sorted_vector_name; }
		virtual size_t      size() const throw()      { return size_; }
		virtual size_t      capacity() const throw()  { return maxi_; }
		virtual void        free() throw()            { _free(); }
		virtual void        release() throw()         { _release(); }
		
		inline void swap_with( sorted_vector &other ) throw()
		{
			cswap( size_, other.size_ );
			cswap( maxi_, other.maxi_ );
			cswap( addr_, other.addr_ );
			cswap( item_, other.item_ );
		}
		
		virtual void        reserve( size_t n )
		{
			if( n > 0 )
			{
				sorted_vector other( maxi_ + n, as_capacity );
				duplicate_into( other );
				swap_with(other);
			}
		}
		
		
		virtual bool search( param_type args ) const throw()
		{
			size_t indx=0;
			return core::locate<const_type,const_type,COMPARATOR&>( &args, addr_, size_, indx, comp_);
		}
		
		virtual bool insert( param_type args )               
		{
			size_t indx=0;
			if( core::locate<const_type,const_type,COMPARATOR&>( &args, addr_, size_, indx, comp_) )
			{
				return false;
			}
			else
			{
				uint64_t      stk[ YOCTO_U64_FOR_ITEM(T) ];
				mutable_type *tmp = _cast::trans<mutable_type,uint64_t>(stk);
				new (tmp) T(args);
				if( size_ >= maxi_ )
				{ 
					try 
					{
						sorted_vector vec( this->next_capacity(maxi_), as_capacity );
						this->duplicate_into(vec);
						this->swap_with(vec);
					}
					catch(...){ destruct(tmp); } 
				}
				core::insert<mutable_type>( tmp, addr_, size_, indx );
				++size_;
				return true;
			}
		}
		
		virtual bool remove( param_type args ) throw() 
		{
			size_t indx=0;
			if( core::locate<const_type,const_type,COMPARATOR&>( &args, addr_, size_, indx, comp_) )
			{
				mutable_type *target = &addr_[indx];
				destruct(target);
				memmove((void*)target,(void*)(target+1), (--size_-indx) * sizeof(T) );
				return true;
			}
			else 
			{
				return false;
			}
			
		}
		
		inline const_type & operator[]( size_t index ) const throw()
		{
			assert( index>0 );
			assert( index <= size_ );
			return item_[ index ];
		}
		
        inline friend std::ostream & operator<<( std::ostream &os, const sorted_vector<T> &a )
		{
			os << "[ ";
			for( size_t i=1; i <= a.size(); ++i ) os <<  a[i] << ' ';
			os << "]'";
			return os;
		}
        
	private:
		YOCTO_DISABLE_ASSIGN(sorted_vector);
		ALLOCATOR          hmem_; //!< handle memory
		size_t             size_; //!< objects inside
		size_t             maxi_; //!< capacity
		mutable_type      *addr_; //!< linear memory
		mutable_type      *item_; //!< addr_-1;
		mutable COMPARATOR comp_; //!< the comparator
		
		inline void _free() throw()
		{
			while( size_ > 0 ) destruct( &addr_[--size_]);
		}
		
		void _release() throw()
		{
			_free();
			static_cast<memory::allocator&>(hmem_).release_as<mutable_type>(addr_,maxi_);
			item_ = addr_ - 1;
		}
		
		inline void duplicate_into( sorted_vector &other ) const
		{
			assert( other.maxi_ >= size_ );
			assert( 0 == other.size_ );
			while( other.size_ < size_ )
			{
				new ( &other.addr_[other.size_] ) mutable_type( addr_[other.size_] );
				++other.size_;
			}
		}
		
	};
	
}

#endif
