#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/exceptions.hpp"
#include <cstring>

namespace yocto
{

    namespace gfx
    {
        stencil:: ~stencil() throw()
        {
            memory::kind<memory::global>::release_as<mask>(masks,nmask);
        }

#define YGFX_STENCIL_CTOR() \
nmask(size), \
masks( memory::kind<memory::global>::acquire_as<mask>(nmask) )

        stencil:: stencil(const size_t n) :
        size(n),
        YGFX_STENCIL_CTOR()
        {
            if(size<=0)
            {
                throw imported::exception("stencil constructor","size=0");
            }
        }


        stencil:: stencil(const stencil &other) :
        size(other.size),
        YGFX_STENCIL_CTOR()
        {
            assert(size>0);
            memcpy(masks,other.masks,size*sizeof(mask));
        }


        stencil::mask & stencil:: operator[](const size_t i) throw()
        {
            assert(i<size);
            return masks[i];
        }

        const stencil::mask & stencil:: operator[](const size_t i) const throw()
        {
            assert(i<size);
            return masks[i];
        }

    }

}


namespace yocto
{
    namespace gfx
    {

        stencil_grad_x:: stencil_grad_x() : stencil(2)
        {
            stencil &self = *this;
            self[0].r = vertex(-1,0); self[0].weight = -1;
            self[1].r = vertex( 1,0); self[1].weight =  1;
        }

        stencil_grad_x:: ~stencil_grad_x() throw()
        {
        }

    }
}

namespace yocto
{
    namespace gfx
    {

        stencil_grad_y:: stencil_grad_y() : stencil(2)
        {
            stencil &self = *this;
            self[0].r = vertex(0,-1); self[0].weight = -1;
            self[1].r = vertex(0, 1); self[1].weight =  1;
        }

        stencil_grad_y:: ~stencil_grad_y() throw()
        {
        }
        
    }
}

namespace yocto
{
    namespace gfx
    {

        stencil_sobel_x:: stencil_sobel_x() : stencil(6)
        {
            stencil &self = *this;
            self[0].r = vertex(-1,-1); self[0].weight = -1;
            self[1].r = vertex(-1, 0); self[1].weight = -2;
            self[2].r = vertex(-1, 1); self[2].weight = -1;

            self[3].r = vertex(1,-1);  self[3].weight =  1;
            self[4].r = vertex(1, 0);  self[4].weight =  2;
            self[5].r = vertex(1, 1);  self[5].weight =  1;

        }

        stencil_sobel_x:: ~stencil_sobel_x() throw()
        {

        }
        
    }
}


namespace yocto
{
    namespace gfx
    {

        stencil_sobel_y:: stencil_sobel_y() : stencil(6)
        {
            stencil &self = *this;
            self[0].r = vertex(-1,-1); self[0].weight = -1;
            self[1].r = vertex( 0,-1); self[1].weight = -2;
            self[2].r = vertex( 1,-1); self[2].weight = -1;

            self[3].r = vertex(-1,1);  self[3].weight =  1;
            self[4].r = vertex( 0,1);  self[4].weight =  2;
            self[5].r = vertex( 1,1);  self[5].weight =  1;

        }

        stencil_sobel_y:: ~stencil_sobel_y() throw()
        {

        }
        
    }
}

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {
        stencil_gauss:: ~stencil_gauss() throw() {}

        stencil_gauss:: stencil_gauss(const size_t w, const float sig) :
        stencil( square_of(2*w+1) )
        {
            assert(sig>0);
            stencil     &self = *this;
            const unit_t W   = unit_t(w);
            const float  den = 2.0f * sig * sig;
            vertex       r;
            size_t       idx = 0;
            float        sum = 0;
            for(r.x=-W;r.x<=W;++r.x)
            {
                const float xx = float(r.x*r.x);
                for(r.y=-W;r.y<=W;++r.y,++idx)
                {
                    const float yy     = float(r.y*r.y);
                    const float weight = expf(-(xx+yy)/den );
                    sum += weight;
                    self[idx].r      = r;
                    self[idx].weight = weight;
                }
            }
            assert(size==idx);
            for(size_t i=0;i<size;++i)
            {
                self[i].weight/=sum;
            }
        }
    }

}






