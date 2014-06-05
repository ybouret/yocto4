#include "yocto/gfx/image.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        image:: ~image() throw()
        {
            
        }
        
        image:: image()
        {
        }
        
        const char image::name[] = "gfx::image";
        
        
        
        image::format:: ~format() throw()
        {
        }
        
        const string & image::format:: key() const throw()
        {
            return name;
        }
        
        image::format:: format(const char *id) :
        name(id)
        {
        }
        
        void image:: declare( format *fmt )
        {
            const format::pointer ptr(fmt);
            if( !db.insert(fmt) )
            {
                throw exception("already has image::format '%s'", ptr->name.c_str());
            }
        }

        surface * image::format:: load_surface(const string &filename, const pixel_format fmt ) const
        {
            const bitmap::pointer bmp( load(filename,fmt.depth, fmt.callback, &fmt) );
            return new surface(fmt,bmp,0);
        }

        static inline void px3(void *addr, const rgba_t &C, const void *)
        {
            rgb_t &O = *(rgb_t *)addr;
            O.r = C.r;
            O.g = C.g;
            O.b = C.b;
        }
        
        const bitmap::pointer image:: format:: load_bitmap3(const string &filename) const
        {
            const bitmap::pointer bmp( load(filename,3,px3,0) );
            return bmp;
        }

        static inline void px4(void *addr, const rgba_t &C, const void *)
        {
            *(rgba_t *)addr = C;
        }
        
        const bitmap::pointer image::format:: load_bitmap4(const string &filename) const
        {
            const bitmap::pointer bmp( load(filename,4,px4,0) );
            return bmp;
        }

        static inline void pxgs(void *addr, const rgba_t &C, const void *)
        {
            *(uint8_t *)addr = conv::greyscale(C.r, C.g, C.b);
        }
        
        const bitmap::pointer image::format::load_greyscale(const string &filename) const
        {
            const bitmap::pointer bmp( load(filename,1,pxgs,0) );
            return bmp;
        }
        
        static inline void pxgs_f(void *addr, const rgba_t &C, const void *)
        {
            *(float *)addr = conv::greyscale_f(C.r, C.g, C.b);
        }
        
        const bitmap::pointer image::format::load_greyscale_f(const string &filename) const
        {
            const bitmap::pointer bmp( load(filename,sizeof(float),pxgs_f,0) );
            return bmp;
        }


        
    }
}
