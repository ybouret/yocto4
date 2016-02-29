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
        
        
        void natural:: kill() throw()
        {
            memIO:: release(byte, maxi);
            size = 0;
        }
        
        natural:: ~natural() throw()
        {
            kill();
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
        
        const uint8_t  natural:: _bit[8] =
        {
            0x01,
            0x02,
            0x04,
            0x08,
            0x10,
            0x20,
            0x40,
            0x80
        };
        
        size_t natural:: bits() const throw()
        {
            if(size)
            {
                const size_t top = size-1;
                const size_t msb = byte[top]; assert(msb);
                for(int i=7;i>=0;--i)
                {
                    if( _bit[i] & msb )
                        return (top<<3) + (i+1);
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
        
        void natural:: __clr() const throw()
        {
            memset((void*)byte,0,maxi);
            (size_t &)size = 0;
        }
        
        
        bool  natural:: is_zero() const throw()
        {
            return size <= 0;
        }
        
        bool  natural:: is_one() const throw()
        {
            return (1 == size) && (1 == byte[0]);
        }
        
        bool natural:: is_two() const throw()
        {
            return (1 == size) && (2 == byte[0]);
        }
        
        bool natural:: is_odd() const throw()
        {
            return (size>0) && ( 0 != (byte[0] & 1) );
        }
        
        bool natural:: is_even() const throw()
        {
            return size <= 0 || !(byte[0]&1);
        }
        
        natural natural:: one()
        {
            natural n;
            n.size    = 1;
            n.byte[0] = 1;
            return n;
        }
        
        natural natural:: two()
        {
            natural n;
            n.size    = 1;
            n.byte[0] = 2;
            return n;
        }
        
        natural natural:: three()
        {
            natural n;
            n.size    = 1;
            n.byte[0] = 3;
            return n;
        }
        
        bool natural::bit( const size_t index ) const throw()
		{
			const size_t n = index >> 3;
			if( n >= size )
				return false;
			else
			{
				return ( byte[n] & _bit[ index & 7 ] ) != 0;
			}
		}
        
        natural:: natural( const string &s) :
        maxi(0),
        size(0),
        byte( memIO::acquire(maxi) )
        {
            YOCTO_CHECK_MPN(this);
            try
            {
                mpn tmp = parse(s);
                xch(tmp);
            }
            catch(...)
            {
                kill();
                throw;
            }
        }
        
        
        natural:: natural( const char *s ) :
        maxi(0),
        size(0),
        byte( memIO::acquire(maxi) )
        {
            YOCTO_CHECK_MPN(this);
            try
            {
                mpn tmp = parse(s);
                xch(tmp);
            }
            catch(...)
            {
                kill();
                throw;
            }
        }
        
        double natural:: to_double() const
        {
            double ans = 0;
            for(size_t i=0;i<size;++i)
            {
                ans *= 256.0;
                ans += byte[i];
            }
            return ans;
        }
        
        
    }
}
