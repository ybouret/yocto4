#include "yocto/graphics/region.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {

        displacement:: ~displacement() throw()
        {
        }

        displacement:: displacement() throw() :
        r(),
        data()
        {
            clear();
        }

        displacement:: displacement(const vertex &p) throw() :
        r(p),
        data()
        {
            clear();
        }

        displacement:: displacement( const displacement &d ) throw() :
        r(d.r),
        data()
        {
            memcpy(data,d.data,sizeof(data));
        }

        displacement & displacement:: operator=(const displacement &d) throw()
        {
            r = d.r;
            memmove(data, d.data, sizeof(data) );
            return *this;
        }

        void displacement:: clear() throw()
        {
            memset(data, 0, sizeof(data) );
        }
    }
}

namespace yocto
{
    namespace graphics
    {
        region:: ~region() throw()
        {
        }

        region:: region() throw() : region_base()
        {
        }

        region::region(const region &R) : region_base(R)
        {
        }

        region & region:: operator=(const region &R)
        {
            region tmp(R);
            swap_with(tmp);
            return *this;
        }


        void region:: load_square(const unit_t w)
        {
            assert(w>=0);
            for(unit_t j=-w;j<=w;++j)
            {
                for(unit_t i=-w;i<=w;++i)
                {
                    const vertex       p(i,j);
                    const displacement d(p);
                    push_back(d);
                }
            }
        }

        void region:: load_disk(const unit_t r)
        {
            assert(r>=0);
            const unit_t r2 = r*r;
            for(unit_t y=-r;y<=r;++y)
            {
                const unit_t y2 = y*y;
                for(unit_t x=-r;x<=r;++x)
                {
                    if(x*x+y2>r2) continue;
                    const vertex       p(x,y);
                    const displacement d(p);
                    push_back(d);
                }
            }

        }

    }

}
