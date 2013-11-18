//! \file net-types.hpp

#ifndef YOCTO_NET_TYPES_INCLUDED
#define YOCTO_NET_TYPES_INCLUDED 1

#include "yocto/code/endian.hpp"
#include <iosfwd>

namespace yocto {
	
	
	namespace network
	{
		
		typedef uint16_t net16_t; //!< assumed big endian 16 bits unsigned integer
		typedef uint32_t net32_t; //!< assumed big endian 32 bits unsigned integer
		typedef uint64_t net64_t; //!< assumed big endian 64 bits unsigned integer
		
		//! assumed big endian 128 bits unsigned integer for IPv6
		class net128_t
		{
		public:
			net128_t() throw();
			~net128_t() throw();
			net128_t( const net128_t & ) throw();
			net128_t &operator=( const net128_t & ) throw();
			
			net128_t( const uint8_t buf[16] ) throw();
			
			uint8_t h[16];
			friend std::ostream & operator<< ( std::ostream &, const net128_t & );
			
		};
		
	}
	
}

#endif
