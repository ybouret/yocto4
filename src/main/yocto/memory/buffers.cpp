#include "yocto/memory/buffers.hpp"

namespace yocto
{
	namespace memory
	{
		local_buffer:: local_buffer( const void *data, size_t size ) throw() :
		data_( data ),
		size_( size )
		{
			assert( !(data==NULL && size>0) );
		}
		
		local_buffer:: ~local_buffer() throw()
		{
		}
		
		size_t       local_buffer:: length() const throw() { return size_; }
		const void * local_buffer:: get_address() const throw() { return data_; }
		
		
	}
}
