#ifndef YOCTO_GFX_OPS_STENCIL_INCLUDED
#define YOCTO_GFX_OPS_STENCIL_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/rawpix.hpp"
#include "yocto/gfx/pixel.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        class stencil
        {
        public:
            YOCTO_PAIR_DECL(YOCTO_TUPLE_STANDARD,mask,vertex,r,float,weight);
            YOCTO_PAIR_END();

            YOCTO_PAIR_DECL(YOCTO_TUPLE_STANDARD,info,float,vmin,float,vmax);
            inline info() throw() : vmin(0), vmax(0) {}
            inline info & operator=(const info &other) throw() { vmin = other.vmin; vmax = other.vmax; return *this; }
            YOCTO_PAIR_END();

            virtual ~stencil() throw();
            explicit stencil(const size_t n,const bool rescaling);
            stencil(const stencil &other);

            const size_t size;
            const mask & operator[](const size_t i) const throw();
            void optimize() throw();
            void display() const;

            //! raw computation source => target, keep track of min/max
            template <typename T, typename U>
            inline void apply(pixmap<T>       &target,
                              const pixmap<U> &source,
                              xpatches        &xps)
            {
                tgt = &target;
                src = &source;
                // run on all patches
                YGFX_SUBMIT(this, (&stencil::run<T,U>) , xps, xp.make<info>() );

                // collect scaling
                global = xps[1].as<info>();
                for(size_t i=xps.size();i>1;--i)
                {
                    const info &local = xps[i].as<info>();
                    global.vmin = min_of(local.vmin,global.vmin);
                    global.vmax = max_of(local.vmax,global.vmax);
                }
            }


            //! computation by channel
            /**
             rescale can be set to false in the case of a mass conserving stencil.
             */
            template <
            typename T,
            typename U,
            size_t   NCH>
            inline void apply(pixmap<T>       &target,
                              const pixmap<T> &source,
                              pixmaps<float>  &channels,
                              xpatches        &xps)
            {
                assert(NCH>0);
                assert(channels.size>=NCH);

                tgt = &target;
                src = &source;
                chn = &channels;

                // run on all patches, all channels
                YGFX_SUBMIT(this, (&stencil::collect<T,U,NCH>) , xps, xp.make<info>() );

                // collect scaling
                global = xps[1].as<info>();
                for(size_t i=xps.size();i>1;--i)
                {
                    const info &local = xps[i].as<info>();
                    global.vmin = min_of(local.vmin,global.vmin);
                    global.vmax = max_of(local.vmax,global.vmax);
                }

                // distribute target
                if(global.vmin<global.vmax)
                {
                    if(is_rescaling)
                    {
                        xps.submit(this, &stencil::expand<T,U,NCH> );
                    }
                    else
                    {
                        xps.submit(this, &stencil::transfer<T,U,NCH> );
                    }
                }
                else
                {
                    target.ldz();
                }
            }



#define YGFX_STENCIL_LOOP(I)              \
const mask   &msk = masks[I];             \
const vertex  pos = center + msk.r;       \
if(source.has(pos))                       \
sum += float(source[pos])*msk.weight;     \
else                                      \
sum += v0*msk.weight


            template <typename U>
            inline float dot(const pixmap<U> &source,
                             const vertex    &center) const throw()
            {
                assert(size>0);
                assert(source.has(center));
                const float v0  = float(source[center]);
                float       sum = 0.0f;
                YOCTO_LOOP_FUNC_(size,YGFX_STENCIL_LOOP,0);
                return sum;
            }

        private:
            YOCTO_DISABLE_ASSIGN(stencil);
            size_t       nmask;
        protected:
            mask        *masks;
        private:
            void        *tgt;
            const void  *src;
            void        *chn;
        public:
            info         global;
            const bool   is_rescaling;

        private:


            //! raw computation
            template <typename T,typename U>
            inline void run( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                pixmap<T>       &target = *static_cast<pixmap<T>      *>(tgt);
                const pixmap<U> &source = *static_cast<const pixmap<U>*>(src);
                info            &local  = xp.as<info>();
                bool             init   = true;
                vertex v;
                for(v.y=xp.upper.y;v.y>=xp.lower.y;--v.y)
                {
                    for(v.x=xp.upper.x;v.x>=xp.lower.x;--v.x)
                    {
                        const float ans = dot<U>(source,v);
                        target[v] = T(ans);
                        if(init)
                        {
                            init = false;
                            local.vmin = local.vmax = ans;
                        }
                        else
                        {
                            if(ans<local.vmin) local.vmin = ans;
                            if(ans>local.vmax) local.vmax = ans;
                        }
                    }
                }
            }

            //! raw computation, NCH channels
            template <typename T,typename U,size_t NCH>
            inline void collect( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(chn);
                //pixmap<T>       &target   = *static_cast<pixmap<T>      *>(tgt);
                const pixmap<T> &source   = *static_cast<const pixmap<T>*>(src);
                pixmaps<float>  &channels = *static_cast<pixmaps<float> *>(chn);
                info            &local  = xp.as<info>();
                bool             init   = true;
                float            sum[NCH];
                float            v0[NCH];
                vertex           v;
                for(v.y=xp.upper.y;v.y>=xp.lower.y;--v.y)
                {
                    for(v.x=xp.upper.x;v.x>=xp.lower.x;--v.x)
                    {
                        // initialize
                        {
                            const U *p = (const U *)&source[v];
                            for(size_t i=0;i<NCH;++i)
                            {
                                sum[i] = 0;
                                v0[i]  = p[i];
                            }
                        }

                        // compute the dot products
                        for(size_t j=0;j<size;++j)
                        {
                            const mask  &msk    = masks[j];
                            const vertex pos    = msk.r + v;
                            const float  weight = msk.weight;

                            if(source.has(pos))
                            {
                                const U *q =(const U *)&source[pos];
                                for(size_t i=0;i<NCH;++i)
                                {
                                    sum[i] += q[i] * weight;
                                }
                            }
                            else
                            {
                                for(size_t i=0;i<NCH;++i)
                                {
                                    sum[i] += v0[i] * weight;
                                }
                            }
                        }

                        // transfert results
                        if(init)
                        {
                            init = false;
                            channels[0][v] = sum[0];
                            local.vmin = local.vmax = sum[0];
                            for(size_t i=1;i<NCH;++i)
                            {
                                const float ans = sum[i];
                                channels[i][v] = ans;
                                local.vmin = min_of(local.vmin,ans);
                                local.vmax = max_of(local.vmax,ans);
                            }
                        }
                        else
                        {
                            for(size_t i=0;i<NCH;++i)
                            {
                                const float ans = sum[i];
                                channels[i][v] = ans;
                                local.vmin = min_of(local.vmin,ans);
                                local.vmax = max_of(local.vmax,ans);
                            }

                        }
                    }
                }

            }


            template <typename T,typename U,size_t NCH>
            inline void transfer( xpatch &xp, lockable & ) throw()
            {
                static const float scale = float(pixel<U>::opaque);
                assert(tgt);
                assert(src);
                assert(chn);
                pixmap<T>             &target   = *static_cast<pixmap<T>      *>(tgt);
                const pixmaps<float>  &channels = *static_cast<pixmaps<float> *>(chn);
                for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
                {
                    for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                    {
                        U *Q = (U *)&target[y][x];
                        for(size_t i=0;i<NCH;++i)
                        {
                            const float f = clamp<float>(0,channels[i][y][x],scale);
                            Q[i] = U(f);
                        }
                    }
                }
            }

            template <typename T,typename U,size_t NCH>
            inline void expand( xpatch &xp, lockable & ) throw()
            {
                static const float scale = float(pixel<U>::opaque);
                assert(tgt);
                assert(src);
                assert(chn);
                pixmap<T>             &target   = *static_cast<pixmap<T>      *>(tgt);
                const pixmaps<float>  &channels = *static_cast<pixmaps<float> *>(chn);
                const float            vmin     = global.vmin;
                const float            vmax     = global.vmax;
                const float            delta    = vmax - vmin;
                for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
                {
                    for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                    {
                        U *Q = (U *)&target[y][x];
                        for(size_t i=0;i<NCH;++i)
                        {
                            const float f = clamp<float>(0,scale*(channels[i][y][x]-vmin)/delta,scale);
                            Q[i] = U(f);
                        }
                    }
                }
            }


        public:
            // with local memory
            class dispatcher : public pixmaps<float>
            {
            public:
                explicit dispatcher(const unit_t W, const unit_t H);
                virtual ~dispatcher() throw();

                template <
                typename T,
                typename U,
                size_t   NCH>
                void apply(const stencil   &S,
                           pixmap<T>       &target,
                           const pixmap<T> &source,
                           xpatches        &xps)
                {
                    stencil &sten = (stencil &)S;
                    sten.apply<T,U,NCH>(target,source,*this,xps);
                }

                inline void operator()(const stencil       &S,
                                       pixmap<float>       &target,
                                       const pixmap<float> &source,
                                       xpatches            &xps)
                {
                    apply<float,float,1>(S,target,source,xps);
                }

                inline void operator()(const stencil     &S,
                                       pixmap<RGB>       &target,
                                       const pixmap<RGB> &source,
                                       xpatches          &xps)
                {
                    apply<RGB,uint8_t,3>(S,target,source,xps);
                }

                

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(dispatcher);
            };

        };


        class stencil_grad_x : public stencil
        {
        public:
            explicit stencil_grad_x();
            virtual ~stencil_grad_x() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_grad_x);
        };


        class stencil_grad_y : public stencil
        {
        public:
            explicit stencil_grad_y();
            virtual ~stencil_grad_y() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_grad_y);
        };


        class stencil_sobel_x : public stencil
        {
        public:
            explicit stencil_sobel_x();
            virtual ~stencil_sobel_x() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_sobel_x);
        };

        class stencil_sobel_y : public stencil
        {
        public:
            explicit stencil_sobel_y();
            virtual ~stencil_sobel_y() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_sobel_y);
        };


        class stencil_scharr_x : public stencil
        {
        public:
            explicit stencil_scharr_x();
            virtual ~stencil_scharr_x() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_scharr_x);
        };

        class stencil_scharr_y : public stencil
        {
        public:
            explicit stencil_scharr_y();
            virtual ~stencil_scharr_y() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_scharr_y);
        };



        class stencil_gauss : public stencil
        {
        public:
            explicit stencil_gauss(const size_t w, const float sig);
            virtual ~stencil_gauss() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_gauss);
        };


    }
}

#endif
