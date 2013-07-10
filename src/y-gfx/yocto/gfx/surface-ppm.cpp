#include "yocto/gfx/surface.hpp"
#include "yocto/ios/ocstream.hpp"


namespace yocto
{
    namespace gfx
    {
        void surface::save_ppm(const string &filename) const
        {
            ios::ocstream fp(filename,false);
            fp("P6\n");
            fp("%u %u\n", unsigned(w), unsigned(h) );
            fp("255\n");
            for(size_t j=0; j<h; ++j )
            {
                for(size_t i=0;i<w;++i)
                {
                    const void   *p = (*this)[j][i];
                    const quad8_t q = format.get_color3( format.get_pixel(p) );
                    fp.append((char*)&q,3);
                }
            }
        }
    }
}