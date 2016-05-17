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
        raster()
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
#define TIFFGetR(abgr) ( (abgr)        & 0xff)
#define TIFFGetG(abgr) (((abgr) >> 8)  & 0xff)
#define TIFFGetB(abgr) (((abgr) >> 16) & 0xff)
#define TIFFGetA(abgr) (((abgr) >> 24) & 0xff)
        
        bitmap  *tiff_format:: load(const string          &filename,
                                    unit_t                 depth,
                                    rgba2data             &proc,
                                    const void            *options) const
        {
            // open file
            I_TIFF      tiff(filename);
            
            // option: set directory
            if(options)
            {
                const size_t d = *(size_t *)options;
                tiff.SetDirectory(d);
            }
            
            // read raster
            tiff.ReadRBGAImage(raster);
            
            // allocate resources
            const int w = tiff.GetWidth();
            const int h = tiff.GetHeight();
            auto_ptr<bitmap> B(new bitmap(depth,w,h) );
            
            // map raster to bitmap
            const uint32_t *p = raster.data;
            for(int j=0;j<h;++j)
            {
                for(int i=0;i<w;++i)
                {
                    const uint32_t P = *(p++);
                    const RGBA   C( TIFFGetR(P), TIFFGetG(P), TIFFGetB(P), TIFFGetA(P));
                    proc(B->get(i,j),C);
                }
            }
            
            return B.yield();
            
            
            
        }
        
        size_t tiff_format:: count_directories(const string &filename) const
        {
            I_TIFF tiff(filename);
            return tiff.CountDirectories();
        }
        
        
        bitmap *tiff_format:: load_bitmap(const string          &filename,
                                          unit_t                 depth,
                                          rgba2data             &proc,
                                          const size_t           indx) const
        
        {
            return load(filename,depth,proc,&indx);
        }
        
        
        
        void tiff_format:: save(const string        &filename,
                                const bitmap        &bmp,
                                data2rgba           &proc,
                                const void          *options) const
        {
            // open file
            O_TIFF tiff(filename);

            // prepare raster
            const unit_t w = bmp.w;
            const unit_t h = bmp.h;
            raster.startup(w*h);
            uint32_t *p = raster.data;
            for(unit_t j=0;j<h;++j)
            {
                for(unit_t i=0;i<w;++i)
                {
                    const RGBA C = proc(bmp.get(i,j));
                    uint32_t  &Q = *(p++);
                    Q  = C.a; Q <<= 8;
                    Q |= C.b; Q <<= 8;
                    Q |= C.g; Q <<= 8;
                    Q |= C.r;
                    assert( TIFFGetR(Q) == C.r );
                    assert( TIFFGetG(Q) == C.g );
                    assert( TIFFGetB(Q) == C.b );
                    assert( TIFFGetA(Q) == C.a );
                }
            }

            // call library
            tiff.WriteRGBAImage(raster,w,h);
        }
        
        
        
        
    }
}
