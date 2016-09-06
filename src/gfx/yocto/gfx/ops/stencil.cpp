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
masks( memory::kind<memory::global>::acquire_as<mask>(nmask) ), \
tgt(0), src(0), chn(0), global()
        
        stencil:: stencil(const size_t n, const bool rescaling) :
        size(n),
        YGFX_STENCIL_CTOR(),
        is_rescaling(rescaling)
        {
            if(size<=0)
            {
                throw imported::exception("stencil constructor","size=0");
            }
        }
        
        
        stencil:: stencil(const stencil &other) :
        size(other.size),
        YGFX_STENCIL_CTOR(),
        is_rescaling(other.is_rescaling)
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

        stencil:: dispatcher:: ~dispatcher() throw()
        {
        }

        stencil:: dispatcher:: dispatcher(const unit_t W, const unit_t H ) :
        pixmaps<float>(4,W,H)
        {
            
        }

    }
}


namespace yocto
{
    namespace gfx
    {
        
        stencil_grad_x:: stencil_grad_x() : stencil(2,true)
        {
            masks[0].r = vertex(-1,0); masks[0].weight = -1;
            masks[1].r = vertex( 1,0); masks[1].weight =  1;
            optimize();
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
        
        stencil_grad_y:: stencil_grad_y() : stencil(2,true)
        {
            masks[0].r = vertex(0,-1); masks[0].weight = -1;
            masks[1].r = vertex(0, 1); masks[1].weight =  1;
            optimize();
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
        
        stencil_sobel_x:: stencil_sobel_x() : stencil(6,true)
        {
            masks[0].r = vertex(-1,-1); masks[0].weight = -1;
            masks[1].r = vertex(-1, 0); masks[1].weight = -2;
            masks[2].r = vertex(-1, 1); masks[2].weight = -1;
            
            masks[3].r = vertex(1,-1);  masks[3].weight =  1;
            masks[4].r = vertex(1, 0);  masks[4].weight =  2;
            masks[5].r = vertex(1, 1);  masks[5].weight =  1;
            optimize();
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
        
        stencil_sobel_y:: stencil_sobel_y() : stencil(6,true)
        {
            masks[0].r = vertex(-1,-1); masks[0].weight = -1;
            masks[1].r = vertex( 0,-1); masks[1].weight = -2;
            masks[2].r = vertex( 1,-1); masks[2].weight = -1;
            
            masks[3].r = vertex(-1,1);  masks[3].weight =  1;
            masks[4].r = vertex( 0,1);  masks[4].weight =  2;
            masks[5].r = vertex( 1,1);  masks[5].weight =  1;
            optimize();
        }
        
        stencil_sobel_y:: ~stencil_sobel_y() throw()
        {
            
        }
        
    }
}


namespace yocto
{
    namespace gfx
    {

        stencil_scharr_x:: stencil_scharr_x() : stencil(6,true)
        {
            masks[0].r = vertex(-1,-1); masks[0].weight = -3;
            masks[1].r = vertex(-1, 0); masks[1].weight = -10;
            masks[2].r = vertex(-1, 1); masks[2].weight = -3;

            masks[3].r = vertex(1,-1);  masks[3].weight =  3;
            masks[4].r = vertex(1, 0);  masks[4].weight =  10;
            masks[5].r = vertex(1, 1);  masks[5].weight =  3;
            optimize();
        }

        stencil_scharr_x:: ~stencil_scharr_x() throw()
        {

        }

    }
}


namespace yocto
{
    namespace gfx
    {

        stencil_scharr_y:: stencil_scharr_y() : stencil(6,true)
        {
            masks[0].r = vertex(-1,-1); masks[0].weight = -3;
            masks[1].r = vertex( 0,-1); masks[1].weight = -10;
            masks[2].r = vertex( 1,-1); masks[2].weight = -3;

            masks[3].r = vertex(-1,1);  masks[3].weight =  3;
            masks[4].r = vertex( 0,1);  masks[4].weight =  10;
            masks[5].r = vertex( 1,1);  masks[5].weight =  3;
            optimize();
        }

        stencil_scharr_y:: ~stencil_scharr_y() throw()
        {

        }

    }
}

namespace yocto
{
    namespace gfx
    {

        static const int xSobel5[5][5] =
        {
            { -1, -2, 0 , 2 , 1 },
            { -4, -8, 0 , 4,  8 },
            { -6, -12, 0, 12, 6 },
            { -4, -8, 0 , 4,  8 },
            { -1, -2, 0 , 2 , 1 }
        };

        stencil_sobel_x5:: stencil_sobel_x5() : stencil(20,true)
        {
            vertex p;
            size_t i = 0;
            for(p.x=-2;p.x<=2;++p.x)
            {
                for(p.y=-2;p.y<=2;++p.y)
                {
                    const int coef = xSobel5[p.y+2][p.x+2];
                    if(coef!=0)
                    {
                        assert(i<size);
                        masks[i].r      = p;
                        masks[i].weight = float(coef);
                        ++i;
                    }
                }
            }
            assert(i==size);
            optimize();
        }

        stencil_sobel_x5:: ~stencil_sobel_x5() throw()
        {
            
        }

        stencil_sobel_y5:: stencil_sobel_y5() : stencil(20,true)
        {
            vertex p;
            size_t i = 0;
            for(p.x=-2;p.x<=2;++p.x)
            {
                for(p.y=-2;p.y<=2;++p.y)
                {
                    const int coef = xSobel5[p.x+2][p.y+2];
                    if(coef!=0)
                    {
                        assert(i<size);
                        masks[i].r      = p;
                        masks[i].weight = float(coef);
                        ++i;
                    }
                }
            }
            assert(i==size);
            optimize();
        }

        stencil_sobel_y5:: ~stencil_sobel_y5() throw()
        {

        }



    }

}

namespace yocto
{
    namespace gfx
    {

        static const int xScharr5[5][5] =
        {
            { -1, -1, 0 , 1 , 1 },
            { -2, -2, 0 , 2,  2 },
            { -3, -6, 0,  6,  3 },
            { -2, -2, 0 , 2,  2 },
            { -1, -1, 0 , 1 , 1 }
        };

        stencil_scharr_x5:: stencil_scharr_x5() : stencil(20,true)
        {
            vertex p;
            size_t i = 0;
            for(p.x=-2;p.x<=2;++p.x)
            {
                for(p.y=-2;p.y<=2;++p.y)
                {
                    const int coef = xScharr5[p.y+2][p.x+2];
                    if(coef!=0)
                    {
                        assert(i<size);
                        masks[i].r      = p;
                        masks[i].weight = float(coef);
                        ++i;
                    }
                }
            }
            assert(i==size);
            optimize();
        }

        stencil_scharr_x5:: ~stencil_scharr_x5() throw()
        {

        }

        stencil_scharr_y5:: stencil_scharr_y5() : stencil(20,true)
        {
            vertex p;
            size_t i = 0;
            for(p.x=-2;p.x<=2;++p.x)
            {
                for(p.y=-2;p.y<=2;++p.y)
                {
                    const int coef = xScharr5[p.x+2][p.y+2];
                    if(coef!=0)
                    {
                        assert(i<size);
                        masks[i].r      = p;
                        masks[i].weight = float(coef);
                        ++i;
                    }
                }
            }
            assert(i==size);
            optimize();
        }

        stencil_scharr_y5:: ~stencil_scharr_y5() throw()
        {
            
        }
        
        
        
    }
    
}


namespace yocto
{
    namespace gfx
    {
        stencil_gauss:: ~stencil_gauss() throw() {}
        
        stencil_gauss:: stencil_gauss(const size_t w, const float sig) :
        stencil( square_of(2*w+1), false )
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
            optimize();
        }
    }
    
}






