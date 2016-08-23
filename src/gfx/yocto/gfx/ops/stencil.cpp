#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/lw-array.hpp"

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

        
        
        const stencil::mask & stencil:: operator[](const size_t i) const throw()
        {
            assert(i<size);
            return masks[i];
        }
        
        static inline int __compare_masks(const stencil::mask &lhs,
                                          const stencil::mask &rhs) throw()
        {
            const vertex L = lhs.r;
            const vertex R = rhs.r;
            if(L.y<R.y)
            {
                return -1;
            }
            else
            {
                if(R.y<L.y)
                {
                    return 1;
                }
                else
                {
                    assert(L.y==R.y);
                    return __compare(L.x,R.x);
                }
            }
        }
        
        void stencil:: optimize() throw()
        {
            lw_array<mask> m(masks,size);
            quicksort(m,__compare_masks);
        }
        
        void stencil:: display() const
        {
            std::cerr << "stencil#=" << size << std::endl;
            for(size_t i=0;i<size;++i)
            {
                std::cerr << masks[i].r << " => " << masks[i].weight << std::endl;
            }
        }
        
    }

}


namespace yocto
{
    namespace gfx
    {

        stencil_grad_x:: stencil_grad_x() : stencil(2)
        {
            masks[0].r = vertex(-1,0); masks[0].weight = -1;
            masks[1].r = vertex( 1,0); masks[1].weight =  1;
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
            masks[0].r = vertex(0,-1); masks[0].weight = -1;
            masks[1].r = vertex(0, 1); masks[1].weight =  1;
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
            masks[0].r = vertex(-1,-1); masks[0].weight = -1;
            masks[1].r = vertex(-1, 0); masks[1].weight = -2;
            masks[2].r = vertex(-1, 1); masks[2].weight = -1;

            masks[3].r = vertex(1,-1);  masks[3].weight =  1;
            masks[4].r = vertex(1, 0);  masks[4].weight =  2;
            masks[5].r = vertex(1, 1);  masks[5].weight =  1;

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
            masks[0].r = vertex(-1,-1); masks[0].weight = -1;
            masks[1].r = vertex( 0,-1); masks[1].weight = -2;
            masks[2].r = vertex( 1,-1); masks[2].weight = -1;

            masks[3].r = vertex(-1,1);  masks[3].weight =  1;
            masks[4].r = vertex( 0,1);  masks[4].weight =  2;
            masks[5].r = vertex( 1,1);  masks[5].weight =  1;

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
                    masks[idx].r      = r;
                    masks[idx].weight = weight;
                }
            }
            assert(size==idx);
            for(size_t i=0;i<size;++i)
            {
                masks[i].weight/=sum;
            }
        }
    }

}






