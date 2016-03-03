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
            uniq( regxel::fast_compare );
        }

        void region:: load_square(const unit_t r)
        {
            assert(r>=0);
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
            for(node_type *node=list_.head;node;node=node->next)
            {
                node->data.r += p;
            }
        }

        void region:: center() throw()
        {
            const unit_t n = list_.size;
            if(n>0)
            {
                vertex G(0,0);
                for(const node_type *node=list_.head;node;node=node->next)
                {
                    G += node->data.r;
                }
                G /= n;
                for(node_type *node=list_.head;node;node=node->next)
                {
                    node->data.r -= G;
                }
            }
        }
        
        
        
    }
}
