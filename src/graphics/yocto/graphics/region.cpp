#include "yocto/graphics/region.hpp"

namespace yocto
{
    namespace graphics
    {

        void region:: simplify() throw()
        {
            list_type stk;
            while(list_.size)
            {
                node_type     *n = list_.pop_front();
                const vertex  &lhs = n->data;
                for(node_type *i=stk.head;i!=NULL;i=i->next)
                {
                    const vertex &rhs = i->data;
                    if(lhs.x==rhs.x&&lhs.y==rhs.y)
                    {
                        pool_.store(n);
                    }
                    else
                    {
                        stk.push_back(n);
                    }
                }
            }
            stk.swap_with(list_);
        }

        void region:: load_block(const vertex &org, unit_t w)
        {
            w = (w<0) ?-w:w;
            for(unit_t j=-w;j<=w;++j)
            {
                for(unit_t i=-w;i<=w;++i)
                {
                    const vertex tmp(org.x+i,org.y+j);
                    push_back(tmp);
                }
            }
        }

        void region:: load_disk(const vertex &org, unit_t r)
        {
            for(unit_t y=-r;y<=r;++y)
            {

            }
        }

        
    }
}