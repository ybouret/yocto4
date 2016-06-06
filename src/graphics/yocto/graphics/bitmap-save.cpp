#include "yocto/graphics/bitmap.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace graphics
    {

#define YGFX_IS_BMP 1
#define YGFX_IS_PPM 2
#define YGFX_IS_EPS 3

        void bitmap:: save( const string &filename, const bool in_color ) const
        {
            if(depth<3) throw exception("bitmap::save(depth<3)");
            string ext = vfs::get_extension(filename); ext.to_lower();
            int    tid = 0;
            if("bmp"==ext)
            {
                tid = YGFX_IS_BMP;
            }
            if("ppm"==ext)
            {
                tid = YGFX_IS_PPM;
            }
            if("eps"==ext)
            {
                tid = YGFX_IS_EPS;
            }
            if(tid<=0) throw exception("bitmap::save(unhandled extension '%s')", ext.c_str());

            

        }

    }

}
