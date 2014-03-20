#ifndef YOCTO_MPA_WORD2MPN_INCLUDED
#define YOCTO_MPA_WORD2MPN_INCLUDED 1

#include "yocto/mpa/natural.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
    namespace mpa
    {
        
        class word2mpn
        {
        public:
            inline word2mpn(uint64_t x) throw() :
            w( swap_le(x) ),
            u(),
            n( *(natural *)&u[0] )
            {
                natural &m = (natural &)n;
                m.byte = (uint8_t *)&w;
                m.size = 8;
                m.maxi = 8;
                m.update();
            }
            
            inline ~word2mpn() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(word2mpn);
            const uint64_t w;
            uint64_t       u[ YOCTO_U64_FOR_ITEM(natural) ];
        public:
            const natural &n;
        };
        
    }
}

#endif

