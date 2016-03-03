#include "yocto/graphics/region.hpp"
#include "yocto/code/bzset.hpp"

#include <cstring>

namespace yocto
{

    namespace graphics
    {
        regxel:: ~regxel() throw() {}


        regxel:: regxel() throw() :
        r(),
        flag(true),
        data()
        {
            memset(data, 0, sizeof(data) );
        }


        regxel:: regxel( const vertex &p ) throw() :
        r(p),
        flag(true),
        data()
        {
            memset(data, 0, sizeof(data) );
        }

        regxel:: regxel(const regxel &other ) throw() :
        r( other.r ),
        flag( other.flag ),
        data()
        {
            memcpy(data,other.data,sizeof(data));
        }

    }



}

namespace yocto
{

    namespace graphics
    {
        region:: region() throw() : regxels() {}
        region:: ~region() throw() {}

        region:: region( const region &other ) : regxels(other)
        {
        }

        region & region:: operator=( const region &other )
        {
            region tmp(other);
            swap_with(tmp);
            return *this;
        }

        void region:: simplify() throw()
        {

        }

        void region:: load_square(const unit_t r)
        {
            assert(r>=0);
            //const size_t len = 2*r+1;
            //ensure( size() + len*len );
            vertex p;
            for(p.y=-r;p.y<=r;++p.y)
            {
                for(p.x=-r;p.x<=r;++p.x)
                {
                    const regxel rg(p);
                    push_back(rg);
                }
            }
        }

        void region:: load_disk(const unit_t r)
        {
            assert(r>=0);
            //const size_t len = 2*r+1;
            //ensure( size() + len*len );
            const unit_t r2 = r*r;
            vertex p;
            for(p.y=-r;p.y<=r;++p.y)
            {
                const unit_t y2=p.y*p.y;
                for(p.x=-r;p.x<=r;++p.x)
                {
                    const unit_t x2 = p.x*p.x;
                    if(r2<(x2+y2)) continue;
                    const regxel rg(p);
                    push_back(rg);
                }
            }
        }

        void region:: shift(const unit_t x, const unit_t y) throw()
        {
            const vertex p(x,y);
            regxels &self = *this;
            for(size_t i=size();i>0;--i) self[i].r += p;
        }

        void region:: center() throw()
        {
            const unit_t n = size();
            
            if(n>0)
            {
                regxels &self = *this;
                vertex G(0,0);
                for(size_t i=n;i>0;--i)
                {
                    G += self[i].r;
                }
                G /= n;
                for(size_t i=n;i>0;--i)
                {
                    self[i].r -= G;
                }
            }
        }
        
        
        
    }
}
