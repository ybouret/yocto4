#ifndef YOCTO_GRAPHICS_OPS_BILINEAR_INCLUDED
#define YOCTO_GRAPHICS_OPS_BILINEAR_INCLUDED 1


#include "yocto/graphics/pixmap.hpp"

namespace yocto
{
    namespace graphics
    {
        struct bilinear
        {
            template <typename T>
            static T mix_quad(const T &q00,
                              const T &q01,
                              const T &q11,
                              const T &q10,
                              const float px0,
                              const float px1,
                              const float py0,
                              const float py1) throw();



            template <typename T>
            static inline void zoom( pixmap<T> &tgt, const pixmap<T> &src ) throw()
            {
                const unit_t w = src.w;
                const unit_t h = src.h;
                const unit_t W = tgt.w;
                const unit_t H = tgt.h;
                assert(W>=w);
                assert(H>=h);
                const unit_t hh = h-2;
                const unit_t HH = H-1;
                const unit_t ww = w-2;
                const unit_t WW = W-1;
                for(unit_t Y=0;Y<H;++Y)
                {
                    const unit_t yd  = hh*Y;
                    const unit_t y0  = yd/HH;
                    const unit_t yr  = yd-y0*HH;
                    const float  py1 = float(yr)/HH; assert(0.0f<=py1);assert(py1<1.0f);
                    const float  py0 = 1.0f - py1;
                    const unit_t y1  = y0+1;
                    assert(y1<h);
                    for(unit_t X=0;X<W;++X)
                    {
                        const unit_t xd = ww*X;
                        const unit_t x0 = xd/WW;
                        const unit_t xr = xd - x0*WW;
                        const float  px1 = float(xr)/WW; assert(0.0f<=px1);assert(px1<1.0f);
                        const float  px0 = 1.0f-px1;
                        const unit_t x1 = x0+1;
                        assert(x1<w);
                        tgt[Y][X] = mix_quad(src[y0][x0],src[y1][x0],src[y1][x1],src[y0][x1],px0,px1,py0,py1);
                    }
                }


            }

        };

        template <>
        float bilinear::mix_quad<float>(const float &q00,
                                        const float &q01,
                                        const float &q11,
                                        const float &q10,
                                        const float px0,
                                        const float px1,
                                        const float py0,
                                        const float py1) throw()
        {
            const float p00=px0*py0;
            const float p01=px0*py1;
            const float p11=px1*py1;
            const float p10=px1*py0;
            return p00*q00+p01*q01+p11*q11+p10*q10;
        }

        template <>
        RGB bilinear::mix_quad<RGB>(const RGB &q00,
                                    const RGB &q01,
                                    const RGB &q11,
                                    const RGB &q10,
                                    const float px0,
                                    const float px1,
                                    const float py0,
                                    const float py1) throw()
        {
            const float p00=px0*py0;
            const float p01=px0*py1;
            const float p11=px1*py1;
            const float p10=px1*py0;

            const float r = p00*gist::unit_float[q00.r] + p01*gist::unit_float[q01.r] + p11*gist::unit_float[q11.r] + p10*gist::unit_float[q10.r];
            const float g = p00*gist::unit_float[q00.g] + p01*gist::unit_float[q01.g] + p11*gist::unit_float[q11.g] + p10*gist::unit_float[q10.g];
            const float b = p00*gist::unit_float[q00.b] + p01*gist::unit_float[q01.b] + p11*gist::unit_float[q11.b] + p10*gist::unit_float[q10.b];

            return RGB(gist::float2byte(r), gist::float2byte(g), gist::float2byte(b));
        }


        template <>
        RGBA bilinear::mix_quad<RGBA>(const RGBA &q00,
                                      const RGBA &q01,
                                      const RGBA &q11,
                                      const RGBA &q10,
                                      const float px0,
                                      const float px1,
                                      const float py0,
                                      const float py1) throw()
        {
            const float p00=px0*py0;
            const float p01=px0*py1;
            const float p11=px1*py1;
            const float p10=px1*py0;

            const float r = p00*gist::unit_float[q00.r] + p01*gist::unit_float[q01.r] + p11*gist::unit_float[q11.r] + p10*gist::unit_float[q10.r];
            const float g = p00*gist::unit_float[q00.g] + p01*gist::unit_float[q01.g] + p11*gist::unit_float[q11.g] + p10*gist::unit_float[q10.g];
            const float b = p00*gist::unit_float[q00.b] + p01*gist::unit_float[q01.b] + p11*gist::unit_float[q11.b] + p10*gist::unit_float[q10.b];
            const float a = p00*gist::unit_float[q00.a] + p01*gist::unit_float[q01.a] + p11*gist::unit_float[q11.a] + p10*gist::unit_float[q10.a];


            return RGBA( gist::float2byte(r), gist::float2byte(g), gist::float2byte(b), gist::float2byte(a) );
        }
        
        
        
        
    }
}

#endif
