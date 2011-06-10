#ifndef YOCTO_MEMORY_BUFFERS_INCLUDED
#define YOCTO_MEMORY_BUFFERS_INCLUDED 1

#include "yocto/memory/buffer.hpp"
#include "yocto/memory/allocator.hpp"

namespace yocto
{
	
	namespace memory
	{
		
		template <typename T,typename ALLOCATOR>
		class buffer_of : public rw_buffer
		{
		public:
			inline explicit buffer_of( size_t items ) :
			size_( items ),
			maxi_( items > 0 ? items : 1 ),
			buff_( kind<ALLOCATOR>:: template acquire_as<T>(maxi_) )
			{
			}
			
			inline virtual ~buffer_of() throw()
			{
				kind<ALLOCATOR>:: template release_as<T>(buff_, maxi_);
				size_ = 0;
			}
			
			inline size_t         size()   const throw() { return size_; }
			inline virtual size_t length() const throw() { return size_ * sizeof(T); }
			
			inline T       * operator()( const ptrdiff_t shift = 0 ) throw()       { return buff_ + shift; }
			inline const T * operator()( const ptrdiff_t shift = 0 ) const throw() { return buff_ + shift; }
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(buffer_of);
			virtual const void *get_address() const throw() { return buff_; }
			size_t size_;
			size_t maxi_;
			T     *buff_;
		};
		
		
	}
	
}

#endif

