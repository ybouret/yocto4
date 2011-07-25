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
	
		uint8_t ro_buffer:: get_byte(size_t index) const throw()
		{
			return index < length() ? ((const uint8_t *)ro())[index] : uint8_t(0);
		}
	}
	
}
