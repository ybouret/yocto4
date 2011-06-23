#include "yocto/net/net-hood.hpp"


namespace yocto {

	namespace network
	{

		dataQ:: dataQ( size_t n ) throw() :
			fragment:: queue( n )
		{
		}
		
		dataQ:: ~dataQ() throw()
		{
		}

	}

}
