#include "yocto/graphics/bitmap.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace graphics
    {

#define YGFX_IS_BMP    1
#define YGFX_IS_PPM    2
#define YGFX_IS_EPS    3

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

            // open file
            ios::wcstream fp(filename);
            const int nx = w;
            const int ny = h;
            // HEADER
            switch (tid) {
                case YGFX_IS_PPM:
                    fp("P6\n%d %d\n255\n",nx,ny);
                    break;

                case YGFX_IS_EPS:
                    fp("%%!PS-Adobe-3.0 EPSF-3.0\n");
                    fp("%%%%Creator: yocto::graphics\n");
                    fp("%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
                    fp("%%%%LanguageLevel: 2\n");
                    fp("%%%%Pages: 1\n");
                    fp("%%%%DocumentData: Clean7Bit\n");
                    fp("%d %d scale\n",nx,ny);
                    fp("%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
                    fp("{currentfile 3 %d mul string readhexstring pop} bind\n",nx);
                    fp("false %d colorimage\n", int(in_color?3:1));
                    break;

                case YGFX_IS_BMP:
#if 0
                    /* Header 10 bytes */
                    putc('B',fptr);
                    putc('M',fptr);
                    size = nx * ny * 3 + 14 + 40;
                    putc((size) % 256,fptr);
                    putc((size / 256) % 256,fptr);
                    putc((size / 65536) % 256,fptr);
                    putc((size / 16777216),fptr);
                    putc(0,fptr); putc(0,fptr);
                    putc(0,fptr); putc(0,fptr);
                    /* Offset to image data */
                    putc(14+40,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
                    /* Information header 40 bytes */
                    putc(0x28,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
                    putc((nx) % 256,fptr);
                    putc((nx / 256) % 256,fptr);
                    putc((nx / 65536) % 256,fptr);
                    putc((nx / 16777216),fptr);
                    putc((ny) % 256,fptr);
                    putc((ny / 256) % 256,fptr);
                    putc((ny / 65536) % 256,fptr);
                    putc((ny / 16777216),fptr);
                    putc(1,fptr); putc(0,fptr); /* One plane */
                    putc(24,fptr); putc(0,fptr); /* 24 bits */
                    /* Compression type == 0 */
                    putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
                    size = nx * ny * 3;
                    putc((size) % 256,fptr);
                    putc((size / 256) % 256,fptr);
                    putc((size / 65536) % 256,fptr);
                    putc((size / 16777216),fptr);
                    putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
                    putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
                    putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); /* No palette */
                    putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr);
#endif
                    break;

                default:
                    break;
            }

            // BINARY DATA


        }

    }

}
