#include "yocto/gfx/image/tiff.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace gfx
    {

        tiff_format:: ~tiff_format() throw()
        {
        }

        tiff_format:: tiff_format() :
        image::format("TIFF"),
        api("y-tiff")
        {
        }

        bool tiff_format:: lossless() const throw()
        {
            return false;
        }

        const char ** tiff_format:: extensions() const throw()
        {
            static const char *__ext[] = { "tif", "tiff", 0 };
            return __ext;
        }
#define TIFFGetR(abgr) ((abgr) & 0xff)
#define TIFFGetG(abgr) (((abgr) >> 8) & 0xff)
#define TIFFGetB(abgr) (((abgr) >> 16) & 0xff)
#define TIFFGetA(abgr) (((abgr) >> 24) & 0xff)
        bitmap  *tiff_format:: load(const string          &filename,
                                    unit_t                 depth,
                                    image::put_rgba_proc   proc,
                                    const void            *args,
                                    const char            *options) const
        {
            const char *fn = filename.c_str();
            void *tiff = api->Open(fn,"r");
            if(!tiff)
            {
                throw exception("can't open '%s'",fn);
            }
            uint32_t w = 0;
            if(!api->GetWidth(tiff,&w))
            {
                throw exception("no width for '%s'",fn);
            }
            uint32_t h = 0;
            if(!api->GetWidth(tiff,&h))
            {
                throw exception("no height for '%s'",fn);
            }
            auto_ptr<bitmap> B(new bitmap(depth,w,h));

            //direct get
            uint32_t *raster = new uint32_t[w*h];
            if(!api->ReadRGBAImage(tiff,w,h,raster))
            {
                delete []raster;
                throw exception("can't read raster for '%s'", fn);
            }

            const uint32_t *p = raster;
            for(size_t j=0;j<h;++j)
            {
                for(size_t i=0;i<w;++i)
                {
                    const uint32_t P = *(p++);
                    const rgba_t   C( TIFFGetR(P), TIFFGetG(P), TIFFGetB(P), TIFFGetA(P));
                    proc(B->get(i,j),C,args);
                }
            }

            delete []raster;
            std::cerr << "TIFF: " << w << "x" << h << std::endl;
            return B.yield();
        }

        void tiff_format:: save(const string        &filename,
                                const bitmap        &bmp,
                                image::get_rgba_proc proc,
                                const void          *args,
                                const char          *options) const
        {
            
        }
        
    }
}