#include "yocto/mpk/natural.hpp"
#include "yocto/mpk/types.hpp"
#include "yocto/string.hpp"
#include "yocto/code/bswap.hpp"
#include <iostream>

namespace yocto
{
    
    namespace mpk
    {

        static inline uint8_t *build(size_t &n)
        {
            static manager &mgr = manager::instance();
            return static_cast<uint8_t *>( mgr.acquire(n) );
        }

        natural:: natural() :
        maxi(0),
        size(0),
        byte( build(maxi) )
        {
        }

        natural:: natural(const size_t num_bytes, const as_capacity_t &) :
        maxi(num_bytes),
        size(0),
        byte( build(maxi) )
        {
        }

        natural:: natural(const natural &other ) :
        maxi( other.size  ),
        size( other.size  ),
        byte( build(maxi) )
        {
            memcpy(byte,other.byte,size);
        }

        natural::natural(word_t w) :
        maxi( sizeof(word_t) ),
        size( sizeof(word_t) ),
        byte( build(maxi) )
        {
            w = swap_le(w);
            memcpy(byte, &w, sizeof(word_t) );
            update();
        }


        word_t natural:: to_word() const throw()
        {
            YOCTO_CHECK_MPN(*this);
            word_t ans = 0;
            for(size_t i=min_of<size_t>(sizeof(word_t),size);i>0;)
            {
                ans <<= 8;
                ans |= byte[--i];
            }
            return ans;
        }

        natural & natural:: operator=(const natural &other)
        {
            natural tmp(other);
            xch(tmp);
            return *this;
        }

        natural & natural:: operator=(const word_t w)
        {
            natural tmp(w);
            xch(tmp);
            return *this;
        }


        natural:: ~natural() throw()
        {
            static manager &mgr = * manager::location();
            mgr.release_as<uint8_t>(byte, maxi);
            size=0;
        }
        
        size_t natural:: length() const throw()
        {
            return size;
        }
        
        const void *natural:: get_address() const throw()
        {
            return byte;
        }
        
        void natural:: update() throw()
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
        
        void natural:: rescan() throw()
        {
            size = maxi;
            update();
        }
        
        void natural:: ldz() throw()
        {
            YOCTO_CHECK_MPN(*this);
            size=0;
        }

        void natural::xch(natural &other) throw()
        {
            cswap(other.maxi, maxi);
            cswap(other.size, size);
            cswap(other.byte, byte);
        }


        static inline bool is0(char C) throw()
        {
            return C == '0';
        }
        
        std::ostream & operator<<( std::ostream &os, const natural &n)
        {
            YOCTO_CHECK_MPN(n);
            os << '<';
            os << n.size << ':';
            string s;
            for(size_t i=0;i<n.size;++i)
            {
                const uint8_t B  = n.byte[i];
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
                
        
    }
}




