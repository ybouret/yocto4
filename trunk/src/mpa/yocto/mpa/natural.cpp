#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/utils.hpp"

#include <cstring>

namespace yocto
{
    namespace mpa
    {
        
        const void * natural:: get_address() const throw()
        {
            return byte;
        }
        
        size_t natural:: length() const throw()
        {
            return size;
        }
        
        natural:: ~natural() throw()
        {
            memIO:: release(byte, maxi);
            size = 0;
        }
        
        natural:: natural() :
        maxi(0),
        size(0),
        byte( memIO::acquire(maxi) )
        {
            YOCTO_CHECK_MPN(this);
        }
        
        natural:: natural(const size_t n, const as_capacity_t &) :
        maxi(n),
        size(0),
        byte( memIO::acquire(maxi) )
        {
            YOCTO_CHECK_MPN(this);
        }
        
        void natural:: xch( natural &other ) throw()
        {
            YOCTO_CHECK_MPN(this);
            YOCTO_CHECK_MPN(&other);
            cswap(maxi,other.maxi);
            cswap(size,other.size);
            cswap(byte,other.byte);
            YOCTO_CHECK_MPN(this);
            YOCTO_CHECK_MPN(&other);
        }
        
        natural:: natural( const natural &other ) :
        maxi( other.size ),
        size( other.size ),
        byte( memIO::acquire(maxi) )
        {
            YOCTO_CHECK_MPN(&other);
            memcpy(byte,other.byte,size);
            YOCTO_CHECK_MPN(this);
        }
        
        
        natural & natural:: operator=( const natural &other )
        {
            natural tmp(other);
            xch(tmp);
            return *this;
        }
        
        size_t natural:: bits() const throw()
        {
            static const size_t msk[8] =
            {
                0x01,
                0x03,
                0x07,
                0x0f,
                0x1f,
                0x3f,
                0x7f,
                0xff
            };
            if(size)
            {
                const size_t msb = byte[size-1]; assert(msb);
                for(size_t i=7;i>0;--i)
                {
                    if( msk[i] & msb )
                        return (size<<3) + i;
                }
                // critical error
            }
            return 0;
        }
        
        void natural:: update() throw()
        {
            while(size>0 && byte[size-1] <= 0 )
                --size;
            YOCTO_CHECK_MPN(this);
        }
        
        void natural:: rescan() throw()
        {
            size = maxi;
            update();
        }
        
        void natural:: ldz() throw()
        {
            size = 0;
        }
        
        void natural:: clear() throw()
        {
            memset(byte,0,maxi);
            size = 0;
        }
        
        
        
        
    }
}
