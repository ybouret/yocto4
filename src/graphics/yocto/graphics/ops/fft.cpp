#include "yocto/graphics/ops/fft.hpp"
#include "yocto/math/fft/fftn.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/graphics/ops/bilinear.hpp"

namespace yocto
{
    namespace graphics
    {
        void fft:: __forward( pixmapz &img ) throw()
        {
            assert(img.model==bitmap::memory_is_global);
            float       *addr  = & (img[0][0].re);
            const size_t nn[4] = { 0, img.w, img.h, 0 };
            math::FourierN<float>::Forward(addr-1, nn, 2);
        }

        void fft:: __reverse( pixmapz &img ) throw()
        {
            assert(img.model==bitmap::memory_is_global);
            float       *addr  = & (img[0][0].re);
            const size_t nn[4] = { 0, img.w, img.h, 0 };
            math::FourierN<float>::Reverse(addr-1, nn, 2);
        }

        fft::fft(const unit_t user_w, const unit_t user_h) :
        data(2*next_power_of_two( size_t(user_w) ),2*next_power_of_two( size_t(user_h) ) ),
        //rect(0,0,data.w/2,data.h/2),
        core(data.w/2,data.h/2)
        {
        }

        fft:: ~fft() throw()
        {
        }


        void fft:: load(const pixmapf &src) throw()
        {
            assert(src.w<=core.w);
            assert(src.h<=core.h);
            bilinear::zoom(core,src); // TODO: optimize if matching sizes
        }

        void fft:: dispatch() throw()
        {
            data.ldz();
            for(unit_t y=0,ym=data.h;y<core.h;++y)
            {
                --ym;
                for(unit_t x=0,xm=data.w;x<core.w;++x)
                {
                    --xm;
                    const float u   = core[y][x];
                    data[y][x] .re  = u;
                    data[y][xm].re  = u;
                    data[ym][x].re  = u;
                    data[ym][xm].re = u;
                }
            }
        }

        void fft:: dispatch(const rectangle area) throw()
        {
            data.ldz();
            assert(core.contains(area));
            const unit_t wtop = data.w-1;
            const unit_t htop = data.h-1;
            for(unit_t y=area.lower.y;y<=area.upper.y;++y)
            {
                const unit_t ym = htop-y;
                for(unit_t x=area.lower.x;x<=area.upper.x;++x)
                {
                    const unit_t xm = wtop-x;
                    const float u   = core[y][x];
                    data[y][x] .re  = u;
                    data[y][xm].re  = u;
                    data[ym][x].re  = u;
                    data[ym][xm].re = u;
                }
            }

        }

        void fft:: forward() throw()
        {
            __forward(data);
        }

        void fft:: reverse() throw()
        {
            __reverse(data);
        }



    }
}
