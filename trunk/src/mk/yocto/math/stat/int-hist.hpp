#ifndef YOCTO_MK_STAT_IHIST_INCLUDED
#define YOCTO_MK_STAT_IHIST_INCLUDED 1

#include "yocto/container/sequence.hpp"

namespace yocto
{
    namespace math
    {

        template <typename SEQ_DATA>
        inline void i_histogram(sequence<typename SEQ_DATA::type> &bins,
                                sequence<size_t>                  &H,
                                const SEQ_DATA                    &data)
        {
            bins.free();
            H.free();

            typename SEQ_DATA::const_iterator d_end = data.end();
            typename SEQ_DATA::const_iterator d_ini = data.begin();
            if(d_ini==d_end)
                return;


        }
    }
}

#endif
