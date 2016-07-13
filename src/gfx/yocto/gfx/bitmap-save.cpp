#include "yocto/gfx/bitmap.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace gfx
    {

#define YGFX_IS_BMP    1
#define YGFX_IS_PPM    2
#define YGFX_IS_EPS    3

        void bitmap:: save( const string &filename, const bool in_color ) const
        {
            if(depth<3) throw exception("bitmap::save(depth<3)");
            string ext = vfs::get_extension(filename); ext.to_lower();
            int    tid = 0;
            bool   swp = false;

            if("bmp"==ext)
            {
                tid = YGFX_IS_BMP;
            }

            if("ppm"==ext)
            {
                tid = YGFX_IS_PPM;
                swp = true;
            }

            if("eps"==ext)
            {
                tid = YGFX_IS_EPS;
                swp = true;
            }

            if(tid<=0) throw exception("bitmap::save(unhandled extension '%s')", ext.c_str());

            // open file
            ios::wcstream fp(filename);
            const int     nx   = w;
            const int     ny   = h;
            int           size = 0;

            // PROLOG
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
                    /* Header 10 bytes */
                    fp.write('B');
                    fp.write('M');
                    size = nx * ny * 3 + 14 + 40;
                    fp.write(size%256);
                    fp.write((size/256)%256);
                    fp.write((size / 65536) % 256);
                    fp.write((size / 16777216));
                    fp.write(0); fp.write(0); fp.write(0); fp.write(0);

                    /* Offset to image data */
                    fp.write(14+40); fp.write(0); fp.write(0); fp.write(0);

                    /* Information header 40 bytes */
                    fp.write(0x28); fp.write(0); fp.write(0); fp.write(0);
                    fp.write((nx) % 256);
                    fp.write((nx / 256) % 256);
                    fp.write((nx / 65536) % 256);
                    fp.write((nx / 16777216));
                    fp.write((ny) % 256);
                    fp.write((ny / 256) % 256);
                    fp.write((ny / 65536) % 256);
                    fp.write((ny / 16777216));
                    fp.write(1); fp.write(0); /* One plane */
                    fp.write(24); fp.write(0); /* 24 bits */
                    /* Compression type == 0 */
                    fp.write(0); fp.write(0); fp.write(0); fp.write(0);
                    size = nx * ny * 3;
                    fp.write((size) % 256);
                    fp.write((size / 256) % 256);
                    fp.write((size / 65536) % 256);
                    fp.write((size / 16777216));
                    fp.write(1); fp.write(0); fp.write(0); fp.write(0);
                    fp.write(1); fp.write(0); fp.write(0); fp.write(0);
                    fp.write(0); fp.write(0); fp.write(0); fp.write(0); /* No palette */
                    fp.write(0); fp.write(0); fp.write(0); fp.write(0);
                    break;

                default:
                    break;
            }

            // BINARY DATA
            int       linelength = 0;
            const int top        = ny-1;
            for(int j=0;j<ny;++j)
            {
                const int rowindex = swp ? (top-j) : j;
                for(int i=0;i<nx;++i)
                {
                    const uint8_t *C = static_cast<const uint8_t *>(get(i,rowindex));
                    const uint8_t  r = C[0];
                    const uint8_t  g = C[1];
                    const uint8_t  b = C[2];
                    const uint8_t  m = gist::greyscale1(r,g,b);
                    switch (tid) {
                        case YGFX_IS_BMP:
                            if(in_color)
                            {
                                fp.write(b); fp.write(g); fp.write(r);
                            }
                            else
                            {
                                fp.write(m); fp.write(m); fp.write(m);
                            }
                            break;

                        case YGFX_IS_PPM:
                            if(in_color)
                            {
                                fp.write(r); fp.write(g); fp.write(b);
                            }
                            else
                            {
                                fp.write(m); fp.write(m); fp.write(m);
                            }
                            break;

                        case YGFX_IS_EPS:
                            if(in_color)
                            {
                                fp("%02x%02x%02x",r,g,b);
                                linelength += 6;
                            }
                            else
                            {
                                fp("%02x",m);
                                linelength+=2;
                            }
                            if (linelength >= 72 || linelength >= nx) {
                                fp("\n");
                                linelength = 0;
                            }
                            break;

                        default:
                            break;
                    }
                }
            }

            // EPILOG
            switch(tid)
            {
                case YGFX_IS_EPS:
                    fp("\n%%%%EOF\n");
                    break;
                    
                default:
                    break;
            }
            
        }
        
    }
    
}
