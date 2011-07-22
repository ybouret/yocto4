#ifndef YOCTO_RX_SYMBOLS_INCLUDED
#define YOCTO_RX_SYMBOLS_INCLUDED 1


#include "yocto/ordered/catalog.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
	namespace regex
	{
		template <typename T>
		struct catalog_of
		{
			typedef catalog<T,comparator<T>,memory::pooled::allocator> type;
		};
		typedef catalog_of<uint8_t>::type symbols;	
		
		
	}
}

#endif

