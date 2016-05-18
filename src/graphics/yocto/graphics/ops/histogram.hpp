#ifndef YOCTO_GRAPHICS_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GRAPHICS_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/rgb.hpp"
#include "yocto/string.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace graphics
    {


        //! one histogram of data
        class Histogram
        {
        public:
            static const size_t bins=256;
            typedef uint64_t    word_type;

            word_type count[256];

            explicit Histogram() throw();
            virtual ~Histogram() throw();

            //! for parallel code
            template <typename T>
            static inline void __update(word_type *arr,
                                        const pixmap<T> &pxm,
                                        const patch &area) throw()
            {
                assert(arr);
                assert(pxm.contains(area));
                for(unit_t j=area.lower.y;j<=area.upper.y;++j)
                {
                    const typename pixmap<T>::row &pj = pxm[j];
                    for(unit_t i=area.lower.x;i<=area.upper.x;++i)
                    {
                        const T &px = pj[i];
                        ++arr[ project<T>(px) ];
                    }
                }
            }

            //! sequential code
            template <typename T>
            inline void update(const pixmap<T> &pxm)
            {
                __update(count, pxm, pxm);
            }

            void collect( const Histogram &H ) throw();

            static void __reset(word_type *arr) throw();
            void reset() throw();                   //! everyting to 0
            void save(const string &hname) const;   //!< save to check
            void save(const char   *hname) const;


            template <typename T>
            void update(const pixmap<T>   &pxm,
                        xpatches          &xps,
                        threading::engine *server)
            {
                src = &pxm;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch    &xp = xps[i];
                    (void) xp.make<Histogram>();
                    xp.enqueue(this, & Histogram::update_cb<T>, server);
                }
                if(server) server->flush();
                for(size_t i=np;i>0;--i)
                {
                    collect( xps[i].as<Histogram>() );
                }
            }



            //! Ostu threshold
            size_t threshold() const throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Histogram);
            const void *src;
            template <typename T>
            inline void update_cb( xpatch &xp, lockable & ) throw()
            {
                assert(src);
                const pixmap<T> &pxm = *static_cast< const pixmap<T> * >(src);
                Histogram &H = xp.as<Histogram>();
                __update<T>(H.count,pxm,xp);
            }
        };


        struct threshold
        {
            enum mode_type
            {
                keep_foreground,
                keep_background
            };

            template <typename T>
            inline static
            void apply(pixmap<T>      &target,
                       const size_t     t,
                       const pixmap<T> &source,
                       mode_type        m ) throw()
            {
                assert(source.w==target.w);
                assert(source.h==target.h);
                const unit_t w = source.w;
                const unit_t h = source.h;

                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Sj = source[j];
                    typename       pixmap<T>::row &Tj = target[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        const T       &src = Sj[i];
                        const size_t   lvl = project<T>(src);
                        T             &tgt = Tj[i];
                        switch(m)
                        {
                            case keep_foreground:
                                if(lvl<=t)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = src;
                                }
                                break;

                            case keep_background:
                                if(t<=lvl)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = invert_color(src);
                                }
                                break;
                        }
                    }
                }
                
            }
        };

    }

}

#endif
