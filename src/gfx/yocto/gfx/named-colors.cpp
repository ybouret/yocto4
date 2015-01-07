#include "yocto/gfx/named-colors.hpp"
#include "yocto/exception.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {
        
        const named_color named_color::reg[] =
        {
#include "colors.hxx"
        };
        
        const size_t named_color::num = sizeof( named_color::reg )/sizeof( named_color::reg[0] );
    
        rgb_t named_color::get(const char *id)
        {
            if(!id) id="";
            const named_color *lo   = reg;
            const named_color *up   = reg + num;
            --up;
            while(lo<=up)
            {
                const named_color *mid = lo + static_cast<ptrdiff_t>(up-lo)/2;
                const int          cmp = strcmp(id,mid->name);
                if(cmp<0)
                {
                    up = mid-1;
                }
                else
                {
                    if(0<cmp)
                    {
                        lo = mid+1;
                    }
                    else
                    {
                        assert(0==cmp);
                        return rgb_t(mid->r,mid->g,mid->b);
                    }
                }
            }
            throw exception("color '%s' not found",id);
        }
        
        rgb_t named_color:: get(const string &id)
        {
            return get( id.c_str() );
        }
        
        rgb_t named_color:: fetch(size_t i) throw()
        {
            const named_color &c = reg[i%num];
            return rgb_t(c.r,c.g,c.b);
        }
    }
    
}

