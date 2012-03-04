#include "yocto/ios/codec.hpp"

namespace yocto
{
    namespace ios
    {
        codec:: ~codec() throw() {}
        codec::  codec() throw() {}
        
        string codec:: to_string()
        {
            string ans;
            char   C=0;
            while( query(C) ) ans.append(C);
            return ans;
        }
        
        string codec:: to_string( const void *data, size_t size )
        {
            assert( !(NULL==data && size>0) );
            reset();
            append( (const char*)data, size );
            flush();
            return to_string();
        }
        
        string codec:: to_string( const memory::ro_buffer &data )
        {
            reset();
            append( data );
            flush();
            return to_string();
        }
        
    }
}
