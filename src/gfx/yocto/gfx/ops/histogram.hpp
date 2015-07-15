#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/code/bzset.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {



        //!
        class histogram
        {
        public:
            static const size_t bins = 256;
            size_t  classes;
            double  count[bins];
            uint8_t bin[bins]; //! 0..classes-1
            double  cdf[bins]; //! 0..classes-1

        


            explicit histogram() throw();
            virtual ~histogram() throw();

            void reset() throw();
            void build_cdf() throw();
            
            template <typename T>
            void build_from( const pixmap<T> &px, uint8_t (*addr2byte)(const T&) ) throw()
            {
                assert(addr2byte);
                const unit_t w = px.w;
                const unit_t h = px.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Rj = px[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        const uint8_t w = addr2byte( Rj[i] );
                        ++count[w];
                    }
                }
                build_cdf();
            }

            size_t threshold() const throw(); //! Otsu
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
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
                       uint8_t        (*addr2byte)(const T&),
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
                        const size_t   lvl = addr2byte(src);
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
