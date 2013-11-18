#include "yocto/gfx/named-rgb.hpp"
#include "yocto/exception.hpp"
#include <cstdlib>
#include <cstring>

namespace yocto
{
    namespace gfx
    {
        
        namespace
        {
            struct named_rgb
            {
                const char *name;
                const uint8_t r,g,b;
                
                static int compare( const void *lhs, const void *rhs ) throw()
                {
                    assert(lhs);
                    assert(rhs);
                    const named_rgb *L = (const named_rgb *)lhs;
                    const named_rgb *R = (const named_rgb *)rhs;
                    return strcmp(L->name, R->name);
                }
            };
            
            static const named_rgb named_reg[] =
            {
#include "./named-rgb.inc"
            };
            
            static const size_t num_named = sizeof(named_reg)/sizeof(named_reg[0]);
        }
        
        rgb_t get_named_rgb( const string &id )
        {
            const named_rgb target = { id.c_str(), 0, 0, 0 };
            const void     *addr   = bsearch(&target,
                                             named_reg,
                                             num_named,
                                             sizeof(named_rgb),
                                             named_rgb::compare);
            if(addr)
            {
                const named_rgb *C = (const named_rgb *)addr;
                return rgb_t(C->r,C->g,C->b);
            }
            throw exception("get_named_rgb(no '%s')", target.name);
        }
        
        
        rgb_t get_named_rgb( const char *id )
        {
            const string ID(id);
            return get_named_rgb(ID);
        }
        
        size_t  get_named_rgb(sequence<string> &names)
        {
            names.free();
            size_t ans = 0;
            for(size_t i=0; i < num_named; ++i )
            {
                const string id( named_reg[i].name );
                names.push_back(id);
                const size_t l = id.size();
                if(l>ans) ans=l;
            }
            return ans;
        }

        rgb_t:: rgb_t( const string &id ) : r(0),g(0),b(0),a(0xff)
        {
            const rgb_t tmp = get_named_rgb(id);
            r=tmp.r;
            g=tmp.g;
            b=tmp.b;
        }
        
        rgb_t:: rgb_t( const char *id ) : r(0),g(0),b(0),a(0xff)
        {
            const rgb_t tmp = get_named_rgb(id);
            r=tmp.r;
            g=tmp.g;
            b=tmp.b;
        }
    }
}
