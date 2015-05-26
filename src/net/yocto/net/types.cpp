#include "yocto/net/types.hpp"
#include <cstring>
#include <iostream>

namespace yocto {

    namespace network {

#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
        net128_t:: net128_t() throw() : h()
        {
            memset( h, 0, sizeof(h) );
        }

        net128_t:: ~net128_t() throw()
        {
        }

        net128_t:: net128_t( const net128_t &other ) throw() : h()
        {
            memcpy( h, other.h, sizeof(h) );
        }

        net128_t & net128_t:: operator=( const net128_t &other ) throw()
        {
            if( this != &other )
            {
                memcpy( h, other.h, sizeof(h) );
            }
            return *this;
        }

        net128_t:: net128_t( const uint8_t buf[16] ) throw() :
        h()
        {
            assert( buf );
            memcpy( h, buf, sizeof(h) );
        }
        
        std::ostream & operator<< ( std::ostream &os, const net128_t &x )
        {
            for( size_t i=0; i < 16; ++i )
            {
                os << int(x.h[i]);
            }
            return os;
        }
        
    }
    
}
