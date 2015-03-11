#ifndef YOCTO_SEQUENCE_VECTOR_INCLUDED
#define YOCTO_SEQUENCE_VECTOR_INCLUDED 1


#include "yocto/container/sequence.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/bitwise.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/sequence/array.hpp"

#include <cstring>

namespace yocto
{
	namespace hidden { extern const char vector_name[]; }
	
	template <typename T, typename ALLOCATOR = memory::global::allocator >
	class vector  : public virtual sequence<T>, public virtual array<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		enum 
		{
			bitwise_ops = support_bitwise_ops<mutable_type>::value,
			no_destruct = support_no_destruct<mutable_type>::value,
			c_style_ops = support_c_style_ops<mutable_type>::value
		};
		
		
		explicit vector() throw() : hmem_(),  size_(0), maxi_(0), item_(NULL) {--item_;}
		virtual ~vector() throw() { this->release(); }
		
		
		vector( const vector &other ) : hmem_(), size_(0), maxi_(other.size_), 
        item_( hmem_.template acquire_as<mutable_type>( maxi_ )-1 )
		{
			assert(maxi_>=other.size_);
			_duplicate(other);
		}
		
		vector( const vector &other, size_t extra ) : hmem_(), size_(0), maxi_(other.size_+extra),
        item_( hmem_.template acquire_as<mutable_type>( maxi_ )-1 )
		{
			assert(maxi_>=other.size_+extra);
			_duplicate(other);
		}
		
		explicit vector( size_t n, const as_capacity_t & ) : hmem_(), size_(0), maxi_(n),
        item_( hmem_.template acquire_as<mutable_type>( maxi_ )-1 )
		{
			assert( maxi_>=n);
		}
		
		inline vector & operator=( const vector &other )
		{
			vector tmp( other );
			swap_with(tmp);
			return *this;
		}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw() { return hidden::vector_name; }
		virtual size_t size()     const throw() { return size_; }
		virtual size_t capacity() const throw() { return maxi_; }
		
		inline void swap_with( vector &other ) throw()
		{
			cswap(size_,other.size_);
			cswap(maxi_,other.maxi_);
			cswap(item_,other.item_);
		}
		
		virtual void free()    throw() { _free( int2type<no_destruct>() ); }
		virtual void release() throw() { _release(); }
		virtual void reserve( size_t n ) { if(n>0) _reserve(n, int2type<bitwise_ops>() ); }
		
		//======================================================================
		// sequence interface
		//======================================================================
		virtual void push_back( param_type obj )
		{
			if( size_ < maxi_ ) { 
				new ( &item_[size_+1] ) mutable_type(obj);
				++size_;
			}
			else {
				vector tmp( *this, this->next_increase(maxi_) );
				tmp.push_back(obj);
				swap_with(tmp);
			}
		}
		virtual void pop_back() throw()           { assert(size_>0); destruct( &item_[size_--] ); }
		virtual void push_front( param_type obj ) { _push_front(obj,int2type<bitwise_ops>()); }
		virtual void pop_front()                  { assert(size_>0); _pop_front( int2type<bitwise_ops>() ); }
		
		//======================================================================
		// specific interface
		//======================================================================
		explicit vector( size_t n, param_type obj ) : hmem_(), size_(0), maxi_(n),
        item_( hmem_.template acquire_as<mutable_type>( maxi_ )-1 )
		{
			assert( maxi_>=n );
			try
			{
				for( size_t i=1; i <= n; ++i )
				{
					new ( &item_[i] ) mutable_type(obj);
					++size_;
				}
			}
			catch(...)
			{
				_release();
				throw;
			}
		}
		
		inline void make( size_t n, param_type obj ) 
		{
			if( n > size_ )
			{
				vector tmp(n,obj);
				swap_with(tmp);
			}
			else
			{
				_free( int2type<no_destruct>() );
				for( size_t i=1; i <= n; ++i )
				{
					new ( &item_[i] ) mutable_type(obj);
					++size_;
				}
			}
		}
        
        inline void keep( size_t n ) throw()
        {
            while( size_ > n ) pop_back();
        }
		
        virtual void reverse() throw()
        {
            mreverse<mutable_type>(item_+1, size_);
        }
		
	private:
		ALLOCATOR     hmem_;
		size_t        size_;
		size_t        maxi_;
		mutable_type *item_; //! [1..size_]
		
        virtual const type &get_data(size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=size());
            return item_[indx];
        }
        
		//----------------------------------------------------------------------
		// _duplicate
		//----------------------------------------------------------------------
		inline void _duplicate( const vector &other )
		{
			assert( 0==size_ ); assert( maxi_ >= other.size_ );
			try
			{
				for( size_t i=1; i <= other.size_; ++i )
				{
					new ( &item_[i] ) mutable_type( other.item_[i] );
					++size_;
				}
			}
			catch(...)
			{
				_release();
				throw;
			}
		}
		//----------------------------------------------------------------------
		// _free( int2type<no_destruct> )
		//----------------------------------------------------------------------
		inline void _free( const int2type<true>  ) throw() { size_=0; }
		inline void _free( const int2type<false> ) throw() {
			while( size_>0 ) destruct( &item_[size_--] );
		}
		
		inline void _release() throw()
		{
			_free( int2type<no_destruct>() ); assert(0==size_);
			static_cast<memory::allocator&>(hmem_).release_as<mutable_type>(++item_,maxi_);
			assert(NULL==item_);assert(0==maxi_); 
			--item_;
		}
		
		//----------------------------------------------------------------------
		// _reserve | int2type<bitwise_ops>
		//----------------------------------------------------------------------
		void _reserve( size_t n, int2type<true> ) {
			assert(n>0);
			vector tmp( n+maxi_, as_capacity );
			memcpy( (void*)& tmp.item_[1], (void*) &item_[1], size_ * sizeof(T) );
			bswap( maxi_, tmp.maxi_ );
			bswap( item_, tmp.item_ );
		}
		
		void _reserve( size_t n, int2type<false> ) {
			vector tmp( *this, n );
			swap_with(tmp);
		}
		
		//----------------------------------------------------------------------
		// _pop_front | int2type<bitwise_ops>
		//----------------------------------------------------------------------
		inline void _pop_front( int2type<true> ) throw()
		{
			mutable_type *target = &item_[1];
			destruct(target);
			memmove( (void*)target, (void*)(target+1), --size_ * sizeof(T));
		}
		
		inline void _pop_front( int2type<false> ) 
		{
			vector tmp( size_-1, as_capacity );
			for( size_t i=1; i < size_; ++i )
			{
				new ( &tmp.item_[i] ) mutable_type( item_[i+1] );
				++ tmp.size_;
			}
			swap_with(tmp);
		}
		
		//----------------------------------------------------------------------
		// _push_front | int2type<bitwise_ops>
		//----------------------------------------------------------------------
		inline void _push_front( param_type obj, int2type<true> ) 
		{
			if( size_ < maxi_ )
			{
				//--------------------------------------------------------------
				// keep a binary ghost if obj is in this vector
				//--------------------------------------------------------------
				uint64_t      wksp[ YOCTO_U64_FOR_ITEM(T) ];
				void         *addr = _cast::load<uint64_t>( wksp );
				memcpy( &wksp[0], (void*)&obj, sizeof(T));
				mutable_type *target = &item_[1];
				const size_t  nbytes = size_ * sizeof(T);
				
				//--------------------------------------------------------------
				// move and construct
				//--------------------------------------------------------------
				memmove( (void*)(target+1), (void*)target, nbytes );
				try {
					new (target) mutable_type( *_cast::from<type>(addr) );
				}
				catch (...) {
					memmove( (void*)target, (void*)(target+1), nbytes );
					throw;
				}
				++size_;
			}
			else
			{
				vector tmp( this->next_capacity(maxi_), as_capacity );
				_front_store(obj,tmp);
			}
		}
		
		inline void _push_front( param_type obj, int2type<false> ) 
		{
			const size_t n = size_ < maxi_ ? maxi_ : this->next_capacity(maxi_);
			vector tmp( n, as_capacity );
			_front_store(obj,tmp);
		}
		
		inline void _front_store( param_type obj, vector &tmp)
		{
			assert( 0 == tmp.size_ ); assert( tmp.maxi_ > size_ );
			//--------------------------------------------------------------
			// new first item
			//--------------------------------------------------------------
			mutable_type *target = &tmp.item_[1];
			new(target) mutable_type(obj);
			tmp.size_ = 1;
			
			//--------------------------------------------------------------
			// other items
			//--------------------------------------------------------------
			for( size_t i=1; i <= size_; ++i )
			{
				new ( ++target ) mutable_type( item_[i] );
				++tmp.size_;
			}
			
			//--------------------------------------------------------------
			// ok, everything is fine !
			//--------------------------------------------------------------
			swap_with(tmp);			
		}
		
		//----------------------------------------------------------------------
		// access
		//----------------------------------------------------------------------
		virtual const_type &get_front() const throw() { assert(size_>0); return item_[1];     }
		virtual const_type &get_back()  const throw() { assert(size_>0); return item_[size_]; }
		virtual const_type *get_item()  const throw() { return item_; }
		
	};
}

#endif
