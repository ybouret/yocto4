#ifndef YOCTO_MPA_WORD2MPZ_INCLUDED
#define YOCTO_MPA_WORD2MPZ_INCLUDED 1

#include "yocto/mpa/integer.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/code/cast.hpp"

namespace yocto
{
    namespace mpa
    {
        
        class word2mpz
        {
        public:
            explicit word2mpz( int64_t x ) throw() :
            w( swap_le(integer::abs_of(x)) ),
            u(),
            z( *(integer *) _cast::load(&u[0]) )
            {
                (sign_type &)(z.s) = integer::sgn_of(x);
                natural         &m = (natural &)(z.n);
                m.byte             = (uint8_t *)&w;
                m.size             = m.maxi = 8;
                m.update();
            }
            
            inline ~word2mpz() throw()
            {
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(word2mpz);
            const uint64_t  w;
            uint64_t        u[ YOCTO_U64_FOR_ITEM(integer) ];
            
        public:
            integer &z;
        };
    }
    
}

#endif
