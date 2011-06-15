//! \file env.hpp

#ifndef YOCTO_ENV_INCLUDED
#define YOCTO_ENV_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto {


	struct environment 
	{
		
		static bool get( string &value, const string &name );
		static void set( const string &name, const string &value);
		static void get( void (*proc)( const string &name, const string &value, void *), void *args );
		
	};


}

#endif
