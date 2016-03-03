#include "yocto/graphics/region.hpp"

namespace yocto
{
    namespace graphics
    {
        region:: region() throw() : region_base() {}
        region:: ~region() throw() {}

        region:: region( const region &other ) : region_base(other)
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
            uniq( gist::fast_compare );
        }

        void region:: load_square(const unit_t r)
        {
            assert(r>=0);
            vertex p;
            for(p.y=-r;p.y<=r;++p.y)
            {
                for(p.x=-r;p.x<=r;++p.x)
                {
                    push_back(p);
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
                    push_back(p);
                }
            }
        }

        void region:: shift(const unit_t x, const unit_t y) throw()
        {
            const vertex p(x,y);
            for(node_type *node=list_.head;node;node=node->next)
            {
                node->data += p;
            }
        }


    }
}
