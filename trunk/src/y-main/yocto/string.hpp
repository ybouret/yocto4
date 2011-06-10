#ifndef YOCTO_STRING_INCLUDED
#define YOCTO_STRING_INCLUDED 1


#include "yocto/string/basic.hpp"
#include "yocto/code/printf-check.hpp"

namespace yocto
{
	typedef basic::string<char>   string;
	typedef basic::string<utf8_t> wstring;
	
	string vformat( const char *fmt,... ) YOCTO_PRINTF_CHECK(1,2);
}

#endif
