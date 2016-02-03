#include "yocto/mpk/natural.hpp"
#include "yocto/mpk/types.hpp"
#include "yocto/string.hpp"
#include "yocto/code/bswap.hpp"
#include <iostream>

namespace yocto
{
    
    namespace mpk
    {
        _natural:: ~_natural() throw()
        {
        }
        
        size_t _natural:: length() const throw()
        {
            return size;
        }
        
        const void *_natural:: get_address() const throw()
        {
            return byte;
        }
        
        void _natural:: update() throw()
        {
            assert(byte);
            assert(maxi);
            assert(size<=maxi);
            while(size>0)
            {
                const register size_t ipos = size-1;
                if(byte[ipos]!=0) return;
                size=ipos;
            }
            YOCTO_CHECK_MPN(*this);
        }
        
        void _natural:: rescan() throw()
        {
            size = maxi;
            update();
        }
        
        void _natural:: ldz() throw()
        {
            YOCTO_CHECK_MPN(*this);
            size=0;
        }
        
        static inline bool is0(char C) throw()
        {
            return C == '0';
        }
        
        std::ostream & operator<<( std::ostream &os, const _natural &n)
        {
            YOCTO_CHECK_MPN(n);
            os << '<';
            os << n.size << ':';
            string s;
            for(size_t i=0;i<n.size;++i)
            {
                const uint8_t B  = n.byte[i]; assert(B>0);
                const uint8_t lo =  B     & 0x0f;
                s.append( hexa_char[lo]);
                const uint8_t hi = (B>>4) & 0x0f;
                s.append(hexa_char[hi]);
            }
            s.trim(is0);
            mreverse((char*)(s.rw()),s.length());
            os << s;
            os << '>';
            return os;
        }
        
        _natural:: _natural() throw() : maxi(0), size(0), byte(0) {}
        
        
        
    }
}

#include "yocto/code/endian.hpp"

namespace yocto
{
    
    namespace mpk
    {
        natural_word:: ~natural_word() throw()
        {
            YOCTO_CHECK_MPN(*this);
        }
        
        natural_word:: natural_word(const word_t x) throw() :
        _natural(),
        w( swap_le(x) )
        {
            maxi = sizeof(word_t);
            size = sizeof(word_t);
            byte = (uint8_t *) &w;
            update();
        }
        
        natural_word:: natural_word(const natural_word &other ) throw() :
        _natural(),
        w( other.w )
        {
            maxi = sizeof(word_t);
            size = other.size;
            byte = (uint8_t *) &w;
        }
        
        natural_word & natural_word:: operator=(const  natural_word &other ) throw()
        {
            size = other.size;
            w    = other.w;
            return *this;
        }
    }
    
}


#include "yocto/code/bswap.hpp"

namespace yocto
{
    
    namespace mpk
    {
        
        natural:: natural( const size_t num_bytes, const as_capacity_t &) :
        _natural()
        {
            startup(num_bytes);
        }
        
        void natural:: startup(size_t num_bytes)
        {
            static manager &mgr = manager::instance();
            assert(0==maxi);
            assert(0==size);
            assert(0==byte);
            maxi = num_bytes;
            byte = static_cast<uint8_t*>(mgr.acquire(maxi));
        }
        
        natural:: ~natural() throw()
        {
            YOCTO_CHECK_MPN(*this);
            static manager &mgr = *manager::location();
            size = 0;
            mgr.release_as<uint8_t>(byte,maxi);
        }
        
        natural:: natural() :
        _natural()
        {
            startup(0);
        }
        
        void natural::xch(natural &other) throw()
        {
            cswap(maxi,other.maxi);
            cswap(size,other.size);
            cswap(byte,other.byte);
        }
        
        
        natural:: natural( const natural &other ) :
        _natural()
        {
            startup(other.size);
            memcpy(byte,other.byte,size=other.size);
        }
        
        natural & natural:: operator=(const natural &other)
        {
            natural tmp(other);
            xch(tmp);
            return *this;
        }
        
        natural:: natural(const word_t w) :
        _natural()
        {
            startup(sizeof(w));
            const word_t x = swap_le(w);
            memcpy(byte,&x,size=sizeof(x));
            update();
        }
        
        natural & natural:: operator=(const word_t w)
        {
            natural tmp(w);
            xch(tmp);
            return *this;
        }
        
        
    }
    
    
}


