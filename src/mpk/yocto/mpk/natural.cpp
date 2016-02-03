#include "yocto/mpk/natural.hpp"
#include "yocto/mpk/types.hpp"
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

        std::ostream & operator<<( std::ostream &os, const _natural &n)
        {
            YOCTO_CHECK_MPN(n);
            os << '<';
            os << n.size << ':';
            for(size_t i=0;i<n.size;++i)
            {
                const uint8_t B  = n.byte[i]; assert(B>0);
                const uint8_t lo =  B     & 0x0f;
                if(lo>0) os << hexa_char[lo];
                const uint8_t hi = (B>>4) & 0x0f;
                if(hi>0) os << hexa_char[hi];
            }
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
        natural_word:: ~natural_word() throw() {}

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



