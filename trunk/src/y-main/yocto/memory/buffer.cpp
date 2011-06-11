#include "yocto/memory/buffer.hpp"

namespace yocto
{
	
	namespace memory
	{
		ro_buffer:: ~ro_buffer() throw() {}
		ro_buffer::  ro_buffer() throw() {}
		
		
		const void * ro_buffer:: ro() const throw()
		{
			const void *ptr = get_address();
			assert( !(NULL==ptr && length()>0) );
			return ptr;
		}
		
		rw_buffer::  rw_buffer() throw() {}
		rw_buffer:: ~rw_buffer() throw() {}
		
		void * rw_buffer:: rw() throw()
		{
			return (void *) ro();
		}
		
	}
	
}
