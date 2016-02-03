#ifndef YOCTO_MPK_WORD2MPN
#define YOCTO_MPK_WORD2MPN 1


#include "yocto/mpk/natural.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/cast.hpp"

namespace yocto
{
    namespace mpk
    {
        class word2mpn
        {
        public:
            inline  word2mpn(const word_t x) throw() :
            word( swap_le(x) ),
            wksp(),
            n( *_cast::from<natural>( &wksp[0] ) )
            {
                natural &m = (natural &)n;
                m.maxi = sizeof(word_t);
                m.size = sizeof(word_t);
                m.byte = (uint8_t *)&word;
                m.update();
            }

            inline ~word2mpn() throw() {}
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(word2mpn);
            const word_t word;
            uint64_t     wksp[ YOCTO_U64_FOR_ITEM(natural) ];
        public:
            const natural &n;
        };

    }

}

#endif
