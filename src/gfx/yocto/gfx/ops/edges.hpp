#ifndef YOCTO_GFX_OPS_EDGES_INCLUDED
#define YOCTO_GFX_OPS_EDGES_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/gfx/pixel.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        class edges : public pixmap<float>
        {
        public:
            static const vertex shift[9];
            static const float  sobel1[9];
            static const float  sobel2[9];

            pixmap<float> I;

            explicit edges(const unit_t W,const unit_t H);
            virtual ~edges() throw();

            template <typename T>
            void build_from(const pixmap<T> &source,
                            xpatches        &xps)
            {
                // build intensity map
                src = &source;
                xps.submit(this, & edges::buildI<T> );

                // build edges indicator
                YGFX_SUBMIT(this,&edges::buildE,xps,xp.make<float>()=0);

                // get amplitude
                amplitude = xps[1].as<float>();
                for(size_t i=xps.size();i>1;--i)
                {
                    amplitude = max_of(amplitude,xps[i].as<float>());
                }

                // normalize
                if(amplitude>0)
                {
                    xps.submit(this, & edges::finalize );
                }
                else
                {
                    ldz();
                }
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(edges);
            const void *src;
            float       amplitude;

            //! build intensity by patch
            template <typename T>
            inline void buildI(xpatch &xp, lockable &) throw()
            {
                assert(src);
                const pixmap<T> &source = *static_cast<const pixmap<T>*>(src);
                for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
                {
                    for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                    {
                        I[y][x] = pixel<T>::to_float( source[y][x] );
                    }
                }
            }

            //! build edge representation
            inline void buildE( xpatch &xp, lockable & ) throw()
            {
                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;
                float                vmax     = 0;
                pixmap<float> &self = *this;
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const vertex here(x,y);
                        float sum1 = 0;
                        float sum2 = 0;
                        for(size_t k=0;k<9;++k)
                        {
                            const vertex probe = here + shift[k];
                            float        II    = 0;
                            if(has(probe))
                            {
                                II = I[probe];
                            }
                            else
                            {
                                II = I[here];
                            }
                            sum1 += sobel1[k] * II;
                            sum2 += sobel2[k] * II;
                        }
                        const float value = sqrtf(sum1*sum1+sum2*sum2);
                        self[y][x] = value;
                        if(value>vmax) vmax=value;
                    }
                }
                xp.as<float>() = vmax;
            }

            //! normalize
            inline void finalize(xpatch &xp, lockable & ) throw()
            {
                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;
                const float          vmax     = amplitude;
                pixmap<float>       &self     = *this;

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        self[y][x] /= vmax;
                    }
                }
            }
        };

    }
}

#endif
