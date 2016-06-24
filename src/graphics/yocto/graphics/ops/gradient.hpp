#ifndef YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED
#define YOCTO_GRAPHICS_OPS_GRADIENT_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/pixmap.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {

        //! will compute gradient
        class gradient
        {
        public:

            struct io_data
            {
                float           gmax;
                pixmap<float>  *tgt;
                const void     *src; //!< pixmap<T>
                void           *dst; //!< pixmap<U>
            };


            inline explicit gradient() throw() {}
            inline virtual ~gradient() throw() {}



            template <
            typename T,
            typename U>
            inline float compute(pixmap<U>          &dst,
                                 pixmap<float>      &grd,
                                 const pixmap<T>    &img,
                                 xpatches           &xps,
                                 threading::engine *server)
            {
                const size_t np = xps.size();
                // start computation
                for(size_t i=np;i>0;--i)
                {
                    xpatch  &xp   = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.gmax     = 0;
                    data.tgt      = &grd;
                    data.src      = &img;
                    data.dst      = &dst;
                    xp.enqueue(this, & gradient::evaluate<T>,server);
                }
                if(server) server->flush();

                // reduce max value
                float gmax = 0;
                for(size_t i=np;i>0;--i)
                {
                    gmax = max_of(gmax,xps[i].as<io_data>().gmax);
                }

                // finalize
                if(gmax>0)
                {
                    for(size_t i=np;i>0;--i)
                    {
                        xpatch  &xp   = xps[i];
                        io_data &data = xp.as<io_data>();
                        data.gmax     = 1.0f/gmax;
                        xp.enqueue(this,&gradient::normalize<U>,server);
                    }
                    if(server) server->flush();
                }
                else
                {
                    dst.ldz();
                }

                return gmax;

            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gradient);

            template <typename T>
            inline void evaluate( xpatch &xp, lockable & ) throw()
            {
                io_data         &data  = xp.as<io_data>();
                pixmap<float>   &g     = *(data.tgt);
                const pixmap<T> &s     = *static_cast<const pixmap<T> *>(data.src);
                const vertex     lower = xp.lower;
                const vertex     upper = xp.upper;
                float            gmax  = 0;
                const unit_t     xmax  = g.upper.x;
                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    const unit_t jm  = j-1;
                    const unit_t jp  = j+1;
                    const unit_t jm2 = j-2;
                    const unit_t jp2 = j+2;

                    pixmap<float>::row            &g_j = g[j];
                    const typename pixmap<T>::row &s_j = s[j];
                    const bitmap::position_type    jpos = g.y_position(j);
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        const float  m3s0 = -3.0f*s_j[i];
                        const unit_t im   = i-1;
                        const unit_t ip   = i+1;
                        const unit_t im2  = i-2;
                        const unit_t ip2  = i+2;
                        float        Gy   = 0;
                        switch(jpos)
                        {
                            case bitmap::at_lower:
                                Gy = 4.0f * float(s[jp][i]) + m3s0 - float(s[jp2][i]);
                                break;

                            case bitmap::is_inner:
                                Gy = float(s[jp][i]) - float(s[jm][i]);
                                break;

                            case bitmap::at_upper:
                                Gy = 4.0f * float(s[jm][i]) + m3s0 - float(s[jm2][i]);
                                break;
                        }

                        float Gx = 0;
                        if(i<=0)
                        {
                            Gx = 4.0f*float(s_j[ip]) + m3s0 - float(s_j[ip2]);
                        }
                        else
                        {
                            if(i>=xmax)
                            {
                                Gx = 4.0f*float(s_j[im]) + m3s0 - float(s_j[im2]);
                            }
                            else
                            {
                                Gx = float(s_j[ip])  - float(s_j[im]);
                            }
                        }
                        const float G = sqrtf(Gy*Gy+Gx*Gx);
                        g_j[i] = G;
                        gmax   = max_of(gmax,G);
                    }
                }
                data.gmax = gmax;
            }

            template <typename U>
            inline void normalize( xpatch &xp, lockable &) throw()
            {
                io_data               &data  = xp.as<io_data>(); assert(data.tgt); assert(data.dst);
                const pixmap<float>   &g     = *(data.tgt);
                pixmap<U>             &d     = *static_cast< pixmap<U> *>(data.dst);

                const vertex     lower = xp.lower;
                const vertex     upper = xp.upper;
                const float      gmax  = data.gmax;

                for(unit_t j=upper.y;j>=lower.y;--j)
                {
                    const pixmap<float>::row &g_j = g[j];
                    typename pixmap<U>::row  &d_j = d[j];
                    for(unit_t i=upper.x;i>=lower.x;--i)
                    {
                        d_j[i]   = gist::float_to<U>( gmax*g_j[i] );
                    }
                }
            }
        };

        template <typename T,typename U>
        inline float compute_gradient(pixmap<U>          &dst,
                                      pixmap<float>      &grd,
                                      const pixmap<T>    &img,
                                      xpatches           &xps,
                                      threading::engine *server)
        {
            pixmap<float> g(dst.w,dst.h);
            gradient      G;
            return G.compute(dst,g,img,xps,server);
        }

    }
}

#endif
