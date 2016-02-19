#include "yocto/mpl/natural.hpp"
#include <cstring>

namespace yocto
{
    namespace mpl
    {
        uint8_t * natural:: build(size_t &n)
        {
            static manager &mgr = manager::instance();
            return mgr.acquire(n);
        }

        natural:: ~natural() throw()
        {
            static manager &mgr = *manager::location();
            size=0;
            mgr.release(byte,maxi);
        }

        natural:: natural() :
        maxi(0),
        size(0),
        byte( build(maxi) )
        {
            YOCTO_CHECK_MPN(*this);
        }

        natural:: natural(const word_t x) :
        maxi( sizeof(word_t) ),
        size( sizeof(word_t) ),
        byte( build(maxi) )
        {
            *(word_t *)byte = swap_le(x);
            update();
            YOCTO_CHECK_MPN(*this);
        }

        natural:: natural( const natural &other ) :
        maxi( other.size ),
        size( other.size ),
        byte( build(maxi) )
        {
            YOCTO_CHECK_MPN(other);
            memcpy(byte, other.byte, size);
        }

        natural:: natural(size_t n, const as_capacity_t &) :
        maxi( n ),
        size( n ),
        byte( build(maxi) )
        {

        }

        natural:: natural(const void *buf, const size_t len) :
        maxi(len),
        size(len),
        byte( build(maxi) )
        {
            assert(!(NULL==buf&&len>0));
            memcpy(byte,buf,len);
            update();
        }
        
    }
}
