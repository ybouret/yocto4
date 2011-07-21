#ifndef YOCTO_RX_SYMBOLS_INCLUDED
#define YOCTO_RX_SYMBOLS_INCLUDED 1


#include "yocto/ordered/catalog.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
	namespace regex
	{
		typedef catalog<uint8_t,comparator<uint8_t>,memory::pooled::allocator> symbols;	
		
	}
}

#endif

