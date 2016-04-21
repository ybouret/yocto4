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
        
        static inline double to_double( const char *txt, const char *ctx = NULL )
        {
            return to_real<double>(txt,ctx);
        }
        
        static inline double to_float( const char *txt, const char *ctx = NULL )
        {
            return to_real<float>(txt,ctx);
        }

        //! implemented for int,size,float,double
        template <typename T>
        static T to( const string &txt, const char *ctx = NULL );
        
        template <typename T>
        static inline T to( const char *txt, const char *ctx = NULL )
        {
            const string _txt(txt);
            return to<T>( _txt, ctx );
        }

        //! convert a char to C string representation
        static void   append_cchar(const char c, string &s);

        //! convert bytes to C representation
        static string to_cstring(const string &src);

	};
	
}

#endif
