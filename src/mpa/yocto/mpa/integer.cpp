#include "yocto/mpa/integer.hpp"
#include "yocto/code/bswap.hpp"

#include <iostream>

namespace yocto
{
    namespace mpa
    {
        sign_type int2sign(int s) throw()
        {
            return (s<0) ? __negative : ( (0<s) ? __positive : __zero);
        }

        integer:: ~integer() throw()
        {
        }
        
        integer:: integer() :
        s(__zero),
        n()
        {
        }
        
        integer:: integer( const integer &other ) :
        s( other.s ),
        n( other.n )
        {
        }
        
        void integer:: check() throw()
        {
            if( n.is_zero() ) (sign_type &)s = __zero;
        }
        
        
        integer::integer(const int64_t  x) :
        s( sgn_of(x) ),
        n( abs_of(x) )
        {
        }
        
        
        sign_type integer::sgn_of(const int64_t x) throw()
        {
            return x < 0 ? __negative : ( x>0 ? __positive : __zero );
        }
        
        sign_type integer::neg_of(const sign_type S) throw()
        {
            switch(S)
            {
                case __negative: return __positive;
                case __zero:     return __zero;
                case __positive: return __negative;
            }
        }
        
        uint64_t integer::abs_of(const int64_t x) throw()
        {
            return x < 0 ? -x : x;
        }
        
        
        std::ostream & operator<<( std::ostream &os, const integer &z)
        {
            if(z.s == __negative)
                os << "-";
            os << z.n;
            return os;
        }
        
        integer:: integer( const sign_type S, const natural &N ) :
        s(S),
        n(N)
        {
            check();
        }
        
        void  integer:: xch( integer &other ) throw()
        {
            cswap_const(s,other.s);
            ((natural &)(n)).xch( (natural &)(other.n) );
        }

        integer & integer:: operator=( const integer &other )
        {
            integer tmp(other);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator=( const int64_t other )
        {
            integer tmp(other);
            xch(tmp);
            return *this;
        }
        
    }
}


