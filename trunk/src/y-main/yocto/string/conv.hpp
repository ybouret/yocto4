#ifndef YOCTO_STRING_CONV_INCLUDED
#define YOCTO_STRING_CONV_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	struct strconv
	{
		static size_t to_size( const string &txt, const char *ctx = NULL );
	};
	
}

#endif
