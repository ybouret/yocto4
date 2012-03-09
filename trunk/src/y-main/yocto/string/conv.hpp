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
        
        static inline size_t to_size( const char *txt, const char *ctx = NULL )
        {
            const string _txt(txt);
            return to_size( _txt, ctx );
        }
        
        static inline size_t to_int( const char*txt, const char *ctx = NULL )
        {
            const string _txt(txt);
            return to_int( _txt, ctx );
        }
        
        template <typename T> 
		static T to_real( const char *txt, const char *ctx = NULL )
        {
            const string _txt(txt);
            return to_real<T>( _txt, ctx );
        }
        
        
	};
	
}

#endif
