#ifndef YOCTO_GRAPHICS_OPS_FILTER_INCLUDED
#define YOCTO_GRAPHICS_OPS_FILTER_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/rawpix.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
    namespace graphics
    {

        enum filter_type
        {
            filter_median,
            filter_average
        };

        class filter
        {
        public:
            inline explicit filter() throw() : tgt(0), src(0), fid(filter_median) {}
            inline virtual ~filter() throw() {}

            template <typename T>
            void apply(pixmap<T>         &target,
                       const pixmap<T>   &source,
                       const filter_type  ops,
                       xpatches          &xps,
                       threading::engine *server)
            {
                tgt = &target;
                src = &source;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.enqueue(this, & filter::run<T>, server);
                }
                if(server) server->flush();
            }


        private:
            void       *tgt;
            const void *src;
            filter_type fid;

            template <typename T>
            inline void run( xpatch &xp, lockable & ) throw()
            {
                assert(tgt); pixmap<T>       &target = *static_cast< pixmap<T> *      >(tgt);
                assert(src); const pixmap<T> &source = *static_cast< const pixmap<T>* >(src);
                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;

                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                T arr[9];
                vertex v;
                for(v.y=ymax;v.y>=ymin;--v.y)
                {
                    for(v.x=xmax;v.x>=xmin;--v.x)
                    {
                        size_t count = 0;
                        arr[count++] = source[v];
                        for(size_t k=0;k<8;++k)
                        {
                            const vertex probe = v+gist::delta[k];
                            if(source.has(probe))
                            {
                                arr[count++] = source[probe];
                            }
                        }
                        switch(fid)
                        {
                            case filter_median:
                                target[v] = get_median<T>(arr,count);
                                break;

                            case filter_average:
                                target[v] = get_average<T>(arr,count);
                                break;
                        }
                    }
                }
            }

            template <typename T>
            T get_median( T arr[], const size_t n )
            {
                assert(arr);
                assert(n>0);
                assert(n<=9);
                lw_array<T> ra(arr,n);
                quicksort(ra);
                if( 0 != (n&1) )
                {
                    // odd
                    return ra[1+(n>>1)];
                }
                else
                {
                    size_t idx=n>>1;
                    real_t sum = real_t(ra[idx]);
                    sum += real_t(ra[idx+1]);
                    sum *= 0.5f;
                    return T(sum);
                }
            }

            template <typename T>
            T get_average( T arr[], const size_t n )
            {
                assert(n>0);
                assert(n<=9);
                real_t sum = 0;
                for(size_t i=0;i<n;++i)
                {
                    sum += real_t(arr[i]);
                }
                sum /= n;
                return T(sum);
            }

            
        };
        
    }
}

#endif
