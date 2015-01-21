#ifndef YOCTO_MK_STAT_IHIST_INCLUDED
#define YOCTO_MK_STAT_IHIST_INCLUDED 1

#include "yocto/container/sequence.hpp"


namespace yocto
{
    namespace math
    {

        // Build an histogram of integer values
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

        namespace kernel
        {
            template <typename ITER_B, typename ITER_H>
            inline double ThresholdVar( size_t n, const ITER_B b, const ITER_H h, size_t &count)
            {
                assert(0==count);

                // first pass
                double mu = 0;
                {
                    ITER_B i = b;
                    ITER_H j = h;
                    for(size_t k=n;k>0;--k,++i,++j)
                    {
                        const size_t num = *j;
                        count += num;
                        mu += double(num) * double(*i);
                    }
                    mu /= count;
                }



            }

            template <typename SEQ_B,typename SEQ_H>
            inline double OtsuScore( const SEQ_B &bins, const SEQ_H &H, size_t t ) throw()
            {
                assert(bins.size()==H.size());
                const size_t n = bins.size();
                assert(t<=n+1);
                
            }
            
        }
    }
}

#endif
