#include "yocto/net/net-hood.hpp"
#include "yocto/memory/malloc.hpp"
#include "yocto/utils.hpp"

#include <cstring>

namespace yocto {

	namespace network
	{

		dataQ:: dataQ( size_t n ) throw() :
			fragment:: dataQ( fragment::layout(n, align_log2) )
		{
		}
		
		dataQ:: ~dataQ() throw()
		{
		}

	}

}
