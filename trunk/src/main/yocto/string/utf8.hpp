#ifndef YOCTO_UTF8_INCLUDED
#define YOCTO_UTF8_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	struct UTF8
	{
		static bool        Encode( utf8_t U, string &out );                //!< append  code point to out
		static utf8_t      Decode( const char * &curr, const char *last ); //!< decode (multi-)byte(s)
		
		static string      Encode( const wstring & ws );
		static wstring     Decode( const string  & s  );
		
		
		static const char ValidCharRX[];   //!< UNESCAPED char: 32 to 127, MINUS quote and backslash
		static const char Valid8to11RX[];  //!< 8..11 bits  pattern
		static const char Valid12to16RX[]; //!< 12..16 bits pattern
		static const char Valid17to21RX[]; //!< 17..21 bits pattern
		
		
	};
	
}

#endif
