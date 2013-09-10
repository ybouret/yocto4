#ifndef YOCTO_MEMORY_BUFFERS_INCLUDED
#define YOCTO_MEMORY_BUFFERS_INCLUDED 1

#include "yocto/memory/buffer.hpp"
#include "yocto/memory/allocator.hpp"
#include "yocto/type/args.hpp"
#include <cstring>

namespace yocto
{
	
	namespace memory
	{
		
		template <typename T,typename ALLOCATOR>
		class buffer_of : public rw_buffer
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			const size_t size;
			const size_t bytes;
			
			inline explicit buffer_of( size_t items ) :
			size( items ),
			bytes( size * sizeof(T) ),
			maxi_( items > 0 ? items : 1 ),
			buff_( kind<ALLOCATOR>:: template acquire_as<mutable_type>(maxi_) )
			{
			}
			
			//! copy content
			inline buffer_of( const buffer_of &other ) :
			size(  other.size  ),
			bytes( other.bytes ),
			maxi_( other.maxi_ ),
			buff_( kind<ALLOCATOR>:: template acquire_as<mutable_type>(maxi_) )
			{
				memcpy( buff_, other.buff_, bytes );
			}
			
			
			inline virtual ~buffer_of() throw()
			{
				kind<ALLOCATOR>:: template release_as<T>(buff_, maxi_);
				(size_t&)size = 0;
			}
			
			inline virtual size_t length()   const throw() { return bytes; }
			
			inline type       * operator()( const ptrdiff_t shift = 0 ) throw()       { return buff_ + shift; }
			inline const_type * operator()( const ptrdiff_t shift = 0 ) const throw() { return buff_ + shift; }
			
			inline type       & operator[]( size_t index ) throw()       { assert( index < size ); return buff_[index]; }
			inline const_type & operator[]( size_t index ) const throw() { assert( index < size ); return buff_[index]; }
			
		private:
			YOCTO_DISABLE_ASSIGN(buffer_of);
			virtual const void *get_address() const throw() { return buff_; }
			size_t        maxi_;
			mutable_type *buff_;
		};
		
		
		class local_buffer : public ro_buffer
		{
		public:
			explicit local_buffer( const void *data, size_t size ) throw();
			virtual ~local_buffer() throw();
			
			virtual size_t length() const throw();
			
		private:
			const void  *data_;
			const size_t size_;
			virtual const void *get_address() const throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(local_buffer);
		};
		
	}
	
}

#endif

