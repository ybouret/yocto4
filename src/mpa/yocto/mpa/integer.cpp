#include "yocto/mpa/integer.hpp"

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
        
    }
}


