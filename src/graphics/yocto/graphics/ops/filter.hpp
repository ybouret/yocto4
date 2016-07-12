#ifndef YOCTO_GRAPHICS_OPS_FILTER_INCLUDED
#define YOCTO_GRAPHICS_OPS_FILTER_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/rawpix.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/functor.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace graphics
    {

        
        template <typename T>
        class filter
        {
        public:
            typedef functor<T,TL1(array<T> &)> function;

            inline explicit filter() throw() : tgt(0), src(0), hfn(0) {}
            inline virtual ~filter() throw() {}

            inline void apply(pixmap<T>         &target,
                              const pixmap<T>   &source,
                              function          &fcn,
                              xpatches          &xps,
                              threading::engine *server)
            {
                const size_t np = xps.size();
                tgt = &target;
                src = &source;
                hfn = &fcn;

                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.enqueue(this, & filter::run, server);
                }
                if(server) server->flush();
            }

            template <typename OBJECT_POINTER,typename METHOD_POINTER> inline
            void apply(pixmap<T>         &target,
                       const pixmap<T>   &source,
                       OBJECT_POINTER     host,
                       METHOD_POINTER     method,
                       xpatches          &xps,
                       threading::engine *server)
            {
                function fcn(host,method);
                this->apply(target,source,fcn,xps,server);
            }

            T median( array<T> &ra ) throw()
            {
                assert(ra.size()>0);
                assert(ra.size()<=9);

                const size_t n = ra.size();
                quicksort(ra);
                if( 0 != (n&1) )
                {
                    // odd
                    return ra[1+(n>>1)];
                }
                else
                {
                    size_t idx = n>>1;
                    real_t sum = real_t(ra[idx]);
                    sum += real_t(ra[idx+1]);
                    sum *= 0.5f;
                    return T(sum);
                }
            }

            T average( array<T> &ra ) throw()
            {
                assert(ra.size()>0);
                assert(ra.size()<=9);
                const size_t n = ra.size();
                real_t sum = 0;
                for(size_t i=n;i>0;--i)
                {
                    sum += real_t(ra[i]);
                }
                return T(sum/n);
            }



        private:
            pixmap<T>       *tgt;
            const pixmap<T> *src;
            function        *hfn;

            inline void run( xpatch &xp, lockable & ) throw()
            {
                assert(tgt); pixmap<T>       &target = *static_cast< pixmap<T> *      >(tgt);
                assert(src); const pixmap<T> &source = *static_cast< const pixmap<T>* >(src);
                assert(hfn);

                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;

                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                //uint64_t wksp[ YOCTO_U64_FOR_SIZE( 9*sizeof(T) ) ];
                //T       *arr  = (T*)&wksp[0];
                T        arr[9];
                function &fcn = *hfn;
                vertex   v;
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
                        lw_array<T> data(arr,count);
                        target[v] = fcn(data);
                    }
                }
            }



        };


        template <>
        inline RGB filter<RGB>:: median(array<RGB> &ra) throw()
        {
            const size_t n = ra.size(); assert(n>0); assert(n<=9);
            quicksort(ra,compare_colors<RGB>);
            return ra[n>>1];
            if( (n&1) )
            {
                // odd
                return ra[1+(n>>1)];
            }
            else
            {
                const size_t i = n>>1;
                return average_colors(ra[i],ra[i+1]);
            }
        }

        template <>
        inline RGBA filter<RGBA>:: median(array<RGBA> &ra) throw()
        {
            const size_t n = ra.size(); assert(n>0); assert(n<=9);
            quicksort(ra,compare_colors<RGBA>);
            //return ra[n>>1];
            if( (n&1) )
            {
                // odd
                return ra[1+(n>>1)];
            }
            else
            {
                const size_t i = n>>1;
                return average_colors(ra[i],ra[i+1]);
            }
        }


        template <>
        inline RGB filter<RGB>:: average(array<RGB> &ra) throw()
        {
            return average_colors(ra);
        }

        template <>
        inline RGBA filter<RGBA>:: average(array<RGBA> &ra) throw()
        {
            return average_colors(ra);
        }


        
    }
}

#endif
