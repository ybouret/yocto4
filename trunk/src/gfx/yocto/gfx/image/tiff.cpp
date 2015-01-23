#include "yocto/gfx/image/tiff.hpp"

namespace yocto
{
    namespace gfx
    {
        
        tiff_format:: ~tiff_format() throw()
        {
        }
        
        tiff_format:: tiff_format(const string &tiff_so) :
        image::format("TIFF"),
        api(tiff_so)
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
        
        bitmap  *tiff_format:: load(const string          &filename,
                                    unit_t                 depth,
                                    image::put_rgba_proc   proc,
                                    const void            *args) const
        {
            
            
            return 0;
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
