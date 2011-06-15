#ifndef YOCTO_KR_UUID_INCLUDED
#define YOCTO_KR_UUID_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/random/bits.hpp"

namespace yocto
{
	
	class uuid : public memory::ro_buffer
	{
	public:
		static const size_t __length = 16;
		
		
		explicit uuid();
		virtual ~uuid() throw();
		uuid( const uuid &other ) throw();
		uuid & operator=( const uuid &other ) throw();
		
		virtual size_t length() const throw();
		string         to_string() const;
		
	private:
		uint8_t byte_[ __length ];
		virtual const void *get_address() const throw();
		
	};
	
	
}

#endif
