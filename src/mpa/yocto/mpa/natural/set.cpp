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

        
    }
}
