#include "yocto/graphics/image/tiff.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace graphics
    {
        
        tiff_format:: ~tiff_format() throw()
        {
        }
        
        tiff_format:: tiff_format() :
        image::format("TIFF"),
        api("y-tiff")
        {
            if(api.is_loaded())
            {
                YOCTO_INTERFACE_CHECK(api,Open);
                YOCTO_INTERFACE_CHECK(api,Close);
                YOCTO_INTERFACE_CHECK(api,GetWidth);
                YOCTO_INTERFACE_CHECK(api,GetHeight);
                YOCTO_INTERFACE_CHECK(api,ReadRGBAImage);
                YOCTO_INTERFACE_CHECK(api,SetDirectory);
            }
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
#define TIFFGetR(abgr) ( (abgr)        & 0xff)
#define TIFFGetG(abgr) (((abgr) >> 8)  & 0xff)
#define TIFFGetB(abgr) (((abgr) >> 16) & 0xff)
#define TIFFGetA(abgr) (((abgr) >> 24) & 0xff)
        
        bitmap  *tiff_format:: load(const string          &filename,
                                    unit_t                 depth,
                                    rgba2data             &proc,
                                    const void            *options) const
        {
            const char *fn = filename.c_str();
            if(!api.is_loaded())
            {
                throw exception("tiff_format.load: TIFF plugin is not loaded");
            }
            void *tiff = api->Open(fn,"r");
            
            if(!tiff)
            {
                throw exception("can't open '%s'",fn);
            }
            
            try
            {
                if(options)
                {
                    const uint32_t d = *(const uint32_t *)options;
                    if(!api->SetDirectory(tiff,d))
                    {
                        throw exception("can't SetDirectory %u", unsigned(d) );
                    }
                }
                
                uint32_t w = 0;
                if(!api->GetWidth(tiff,&w))
                {
                    throw exception("no width for '%s'",fn);
                }
                uint32_t h = 0;
                if(!api->GetHeight(tiff,&h))
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
                        const RGBA   C( TIFFGetR(P), TIFFGetG(P), TIFFGetB(P), TIFFGetA(P));
                        proc(B->get(i,j),C);
                    }
                }
                
                delete []raster;
                std::cerr << "TIFF: " << w << "x" << h << std::endl;
                api->Close(tiff);
                return B.yield();
            }
            catch(...)
            {
                assert(tiff);
                api->Close(tiff);
                throw;
            }
        }
        
        uint32_t tiff_format:: count_directories(const string &filename) const
        {
            const char *fn = filename.c_str();
            if(!api.is_loaded())
            {
                throw exception("tiff_format.load: TIFF plugin is not loaded");
            }
            void *tiff = api->Open(fn,"r");
            if(!tiff)
            {
                throw exception("TIFF: cannot open '%s'", fn);
            }
            try
            {
                const uint32_t nd = api->CountDirectories(tiff);
                api->Close(tiff);
                return nd;
            }
            catch(...)
            {
                api->Close(tiff);
                throw;
            }
            
        }
        
        
        bitmap *tiff_format:: load_bitmap(const string          &filename,
                                          unit_t                 depth,
                                          rgba2data             &proc,
                                          const uint32_t         indx) const
        
        {
            return load(filename,depth,proc,&indx);
        }
        
        
        
        void tiff_format:: save(const string        &filename,
                                const bitmap        &bmp,
                                data2rgba           &proc,
                                const void          *options) const
        {
            
        }
        
        
        
        
    }
}
