#ifndef YOCTO_STRING_CONV_INCLUDED
#define YOCTO_STRING_CONV_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	struct strconv
	{
		static size_t to_size( const string &txt, const char *ctx = NULL );
		static int    to_int( const string &txt, const char *ctx = NULL );
		template <typename T> 
		static T to_real( const string &txt, const char *ctx = NULL );
	};
	
}

#endif
