#ifndef YOCTO_GFX_OPS_FILTER_INCLUDED
#define YOCTO_GFX_OPS_FILTER_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/pixel.hpp"
#include "yocto/gfx/xpatch.hpp"

#include "yocto/sequence/lw-array.hpp"
#include "yocto/functor.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace gfx
    {
        class _filter
        {
        public:
            virtual ~_filter() throw();

            static const size_t items= 9;
            static const vertex shift[items];

        protected:
            explicit _filter() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(_filter);
        };

        
        template <typename T>
        class filter
        {
        public:
            typedef functor<T,TL1(array<T> &)> function;

            explicit filter() throw() : pfn(0), tgt(0), src(0) {}
            virtual ~filter() throw() {}

            inline void apply(pixmap<T>         &target,
                              const pixmap<T>   &source,
                              function          &fn,
                              xpatches          &xps,
                              threading::engine *server)
            {
                tgt = &target;
                src = &source;
                pfn = &fn;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch    &xp = xps[i];
                    xp.enqueue(this, & filter<T>::run, server);
                }
                if(server) server->flush();
            }

            template <
            typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void apply(pixmap<T>         &target,
                              const pixmap<T>   &source,
                              HOST_POINTER       host,
                              METHOD_POINTER     method,
                              xpatches          &xps,
                              threading::engine *server)
            {
                function fn(host,method);
                apply(target,source,fn,xps,server);
            }

            //! replace pixel by its average
            inline T average( array<T> &ra ) throw()
            {
                return pixel<T>::average(ra);
            }

            //! replace pixel by its median
            inline T median( array<T> &ra ) throw()
            {
                assert(ra.size()>0);
                quicksort(ra);
                const size_t n = ra.size();
                if( 0 != (n&1) )
                {
                    return ra[1+(n>>1)];
                }
                else
                {
                    const lw_array<T> sub( &ra[n>>1], 2 );
                    return pixel<T>::average(sub);
                }
            }

            //! replace pixel by the its maximum
            inline T dilate( array<T> &ra ) throw()
            {
                assert(ra.size()>0);
                size_t n  = ra.size();
                T      v  = ra[n];
                for(--n;n>0;--n)
                {
                    const T tmp = ra[n];
                    if(v<=tmp) v = tmp;
                }
                return v;
            }

            //! replace pixel by its minimum
            inline T erode( array<T> &ra ) throw()
            {
                assert(ra.size()>0);
                size_t n  = ra.size();
                T      v  = ra[n];
                for(--n;n>0;--n)
                {
                    const T tmp = ra[n];
                    if(tmp<=v) v = tmp;
                }
                return v;
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(filter);
            function        *pfn;
            pixmap<T>       *tgt;
            const pixmap<T> *src;

            inline void run(xpatch   &xp,
                            lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(pfn);
                pixmap<T>       &target = *static_cast< pixmap<T>       *>(tgt);
                const pixmap<T> &source = *static_cast< const pixmap<T> *>(src);
                function        &fn     = *pfn;

                const unit_t ymin = xp.lower.y;
                const unit_t ymax = xp.upper.y;
                const unit_t xmin = xp.lower.x;
                const unit_t xmax = xp.upper.x;

                T arr[_filter::items];
                vertex center;
                for(center.y=ymax;center.y>=ymin;--center.y)
                {
                    for(center.x=xmax;center.x>=xmin;--center.x)
                    {
                        size_t count = 0;
                        for(register size_t k=0;k<_filter::items;++k)
                        {
                            const vertex probe = center + _filter::shift[k];
                            if(source.has(probe))
                            {
                                arr[count++] = source[probe];
                            }
                        }
                        assert(count>0);
                        lw_array<T> ra(arr,count);
                        target[center] = fn(ra);
                    }
                }
            }
        };

        //! filter with own memory
        template <typename T>
        class Filter : public pixmap<T>, public filter<T>
        {
        public:
            typedef typename filter<T>::function function;

            explicit Filter( const unit_t W, const unit_t H ) :
            pixmap<T>(W,H), filter<T>()
            {
            }

            virtual ~Filter() throw() {}

            inline void Apply(pixmap<T>         &target,
                              function          &fn,
                              xpatches          &xps,
                              threading::engine *server)
            {
                pixmap<T> &self = *this;
                self.copy(target);
                this->apply(target,self,fn,xps,server);
            }

            template <
            typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void Apply(pixmap<T>         &target,
                              HOST_POINTER       host,
                              METHOD_POINTER     method,
                              xpatches          &xps,
                              threading::engine *server)
            {
                function fn(host,method);
                Apply(target,fn,xps,server);
            }

            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Filter);
        };


    }
}

#endif
