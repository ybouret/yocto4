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
        flag(true),
        data()
        {
            clear();
        }

        displacement:: displacement(const vertex &p) throw() :
        r(p),
        flag(true),
        data()
        {
            clear();
        }

        displacement:: displacement( const displacement &d ) throw() :
        r(d.r),
        flag(d.flag),
        data()
        {
            memcpy(data,d.data,sizeof(data));
        }

        displacement & displacement:: operator=(const displacement &d) throw()
        {
            r    = d.r;
            flag = d.flag;
            memmove(data, d.data, sizeof(data) );
            return *this;
        }

        void displacement:: clear() throw()
        {
            flag = true;
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


        void region:: shift(const vertex &delta) throw()
        {
            for(node_type *n = list_.head;n;n=n->next)
            {
                n->data.r += delta;
            }
        }

        void region:: simplify() throw()
        {
            list_type stk;
            while(list_.size)
            {
                node_type   *i = list_.pop_front();
                const vertex u = i->data.r;
                bool         found = false;
                for(const node_type *j=stk.head;j;j=j->next)
                {
                    const vertex v = j->data.r;
                    if( (u.x==v.x) && (u.y==v.y) )
                    {
                        found=true;
                        break;
                    }
                }

                if(found)
                {
                    pool_.store(i);
                }
                else
                {
                    stk.push_back(i);
                }
            }
            list_.swap_with(stk);
        }


    }

}
