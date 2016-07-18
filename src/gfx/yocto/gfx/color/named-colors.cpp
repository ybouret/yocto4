#include "yocto/gfx/color/named-colors.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        static const named_color nc_reg[] =
        {
#include "named-colors-def.inc"
        };

        const size_t named_color::count = sizeof(nc_reg)/sizeof(nc_reg[0]);

        RGB named_color::fetch(const size_t indx) throw()
        {
            const named_color &col = nc_reg[indx%count];
            return RGB(col.r,col.g,col.b);
        }

        RGB named_color:: query(const string &name)
        {
            for(size_t i=0;i<count;++i)
            {
                const named_color &col = nc_reg[i];
                if(name==col.name)
                {
                    return RGB(col.r,col.g,col.b);
                }
            }
            throw exception("no name_color '%s'", name.c_str());
        }

        RGB named_color:: query(const char *name)
        {
            const string NAME(name);
            return query(NAME);
        }

        
    }
}