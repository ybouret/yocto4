#include "yocto/mpa/integer.hpp"
#include <iostream>

namespace yocto
{
    namespace mpa
    {
        
        integer:: ~integer() throw()
        {
        }
        
        integer:: integer() :
        s(is_zero),
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
            if( n.is_zero() ) (sign_type &)s = is_zero;
        }
        
        
        integer::integer(const int64_t  x) :
        s( sgn_of(x) ),
        n( abs_of(x) )
        {
        }
        
        
        integer::sign_type integer::sgn_of(const int64_t x) throw()
        {
            return x < 0 ? is_negative : ( x>0 ? is_positive : is_zero );
        }
        
        uint64_t integer::abs_of(const int64_t x) throw()
        {
            return x < 0 ? -x : x;
        }
        
        
        std::ostream & operator<<( std::ostream &os, const integer &z)
        {
            if(z.s == z.is_negative)
                os << "-";
            os << z.n;
            return os;
        }

    }
}


