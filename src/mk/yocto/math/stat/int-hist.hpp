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
            typename SEQ_DATA::const_iterator d     = data.begin();
            if(d==d_end)
                return;

            typename SEQ_DATA::type lo = *d;
            typename SEQ_DATA::type hi = lo;
            while(++d!=d_end)
            {
                typename SEQ_DATA::const_type tmp = *d;
                if(tmp<lo) lo = tmp;
                if(hi<tmp) hi = tmp;
            }
            const size_t n = hi+1-lo;
            bins.ensure(n);
            H.ensure(n);

            for(typename SEQ_DATA::type i=lo;i<=hi;++i)
            {
                bins.push_back(i);
                size_t count = 0;
                for(d=data.begin();d!=d_end;++d)
                {
                    typename SEQ_DATA::const_type tmp = *d;
                    if(tmp==i) ++count;
                }
                H.push_back(count);
            }

        }
    }
}

#endif
