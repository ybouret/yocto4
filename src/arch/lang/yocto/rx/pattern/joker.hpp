#ifndef YOCTO_RX_JOKER_INCLUDED
#define YOCTO_RX_JOKER_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	namespace regex
	{
	
		struct joker
		{
			static const uint32_t _at_least = YOCTO_FOURCC('#','M','I','N');
			static const uint32_t _counting = YOCTO_FOURCC('#','C','N','T');
			
			static pattern *at_least( pattern *p, size_t n ); //!< '*' or '+', p is fully handled, data is &num
			static pattern *counting( pattern *p, size_t nmin, size_t nmax ); //!< {nmin,nmax} or {nmin=nmax}, data is struct { nmin, nmax }
			
		};
		
	}
	
}

#endif
