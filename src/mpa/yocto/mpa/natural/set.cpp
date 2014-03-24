#include "yocto/mpa/natural.hpp"
#include <cstring>

namespace yocto
{
    namespace mpa
    {
        
        
        natural:: natural( uint64_t x ) :
        maxi( sizeof(x) ),
        size( maxi ),
        byte( memIO::acquire(maxi) )
        {
            for(size_t i=0;i<sizeof(x);++i)
            {
                byte[i] = uint8_t(x);
                x >>= 8;
            }
            update();
        }
        
        natural & natural:: operator=( uint64_t x) throw()
        {
            assert(maxi>=sizeof(x));
            size = sizeof(x);
            for(size_t i=0;i<sizeof(x);++i)
            {
                byte[i] = uint8_t(x);
                x >>= 8;
            }
            update();
            return *this;
        }

        
        natural:: natural(const memory::ro_buffer &buf ) :
        maxi( buf.length() ),
        size( maxi ),
        byte( memIO::acquire(maxi) )
        {
            memcpy(byte,buf.ro(),buf.length());
            update();
        }
        
        natural:: natural(const void *buf, size_t len ) :
        maxi( len  ),
        size( maxi ),
        byte( memIO::acquire(maxi) )
        {
            assert(!(buf==0&&len>0));
            memcpy(byte,buf,len);
            update();
        }

        natural & natural:: operator=( const string &s )
        {
            natural tmp = natural::parse(s);
            xch(tmp);
            return *this;
        }
        
        natural & natural:: operator=( const char *s )
        {
            natural tmp = natural::parse(s);
            xch(tmp);
            return *this;
        }

    }
}
