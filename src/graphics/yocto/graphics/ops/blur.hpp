#ifndef YOCTO_GRAPHICS_OPS_BLUR_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLUR_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/xpatch.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {

        class blur
        {
        public:
            explicit blur( float sig);
            virtual ~blur() throw();

            const unit_t  width;    //!< filter: -(width-1)..0..(width-1)
            const unit_t  top;      //!< width-1

            template <
            typename T,
            typename U,
            size_t   NCH>
            inline void apply_x(pixmap<T>         &tgt,
                                const pixmap<T>   &src,
                                xpatches          &xps,
                                threading::engine *server
                                )
            {
                source = &src;
                target = &tgt;
                for(size_t p=xps.size();p>0;--p)
                {
                    xpatch &xp = xps[p];
                    xp.enqueue(this, & blur::eval_x<T,U,NCH>, server);
                }
                if(server) server->flush();
            }

            


        private:
            float        *weight;
            vector<float> __data;
            YOCTO_DISABLE_COPY_AND_ASSIGN(blur);

            void       *target;
            const void *source;
            size_t      counts;

            template <typename T, typename U, size_t NCH>
            inline void eval_x( xpatch &xp, lockable & ) throw()
            {
                assert(target);
                assert(source);
                const pixmap<T> &src  = *static_cast<const pixmap<T>*>(source);
                pixmap<T>       &tgt  = *static_cast< pixmap<T>     *>(target);
                const unit_t     top  = this->top;
                const unit_t     xup  = src.upper.x;

                float sum[NCH];

                for(unit_t j=xp.lower.y;j<=xp.upper.y;++j)
                {
                    const typename pixmap<T>::row &src_j = src[j];
                    typename       pixmap<T>::row &tgt_j = tgt[j];
                    for(unit_t i=xp.lower.x;i<=xp.upper.x;++i)
                    {
                        const unit_t kinf = min_of<unit_t>(top,i);
                        const unit_t ksup = min_of<unit_t>(top,xup-i);

                        // accumulate
                        for(size_t ch=0;ch<NCH;++ch)
                        {
                            sum[ch] = 0;
                        }
                        float acc = 0;

                        for(unit_t k=-kinf;k<=ksup;++k)
                        {
                            const U    *p    = (const U*) &src_j[i+k];
                            const float coef = weight[(k>=0)?k:-k];
                            for(size_t ch=0;ch<NCH;++ch)
                            {
                                sum[ch] += float(p[ch])*coef;
                            }
                            acc += coef;
                        }

                        // transfer
                        U *q = (U*)&tgt_j[i];
                        acc  = 1.0f/acc;
                        for(size_t ch=0;ch<NCH;++ch)
                        {
                            q[ch] = U(sum[ch]*acc);
                        }
                    }
                }
            }
            
        };
        
    }
}

#endif
