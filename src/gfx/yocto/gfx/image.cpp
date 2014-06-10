#include "yocto/gfx/image.hpp"
#include "yocto/exception.hpp"
#include "yocto/fs/vfs.hpp"

namespace yocto
{
    namespace gfx
    {
        
        image:: ~image() throw()
        {
            
        }
        
        image:: image() : db()
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
            assert(fmt);
            const format::pointer ptr(fmt);
            std::cerr << "declare '" << ptr->name << "'" << std::endl;
            if( !db.insert(fmt) )
            {
                throw exception("already has image::format '%s'", ptr->name.c_str());
            }
        }
        
        bitmap * image::load(const string         &filename,
                             unit_t                depth,
                             image::put_rgba_proc  proc,
                             const void           *args) const
        {
            return get_format_for(filename).load(filename,depth,proc,args);
        }
        
        const image::format & image:: get_format_for( const string &filename) const
        {
            for( format::database::const_iterator i = db.begin(); i != db.end(); ++i )
            {
                const format &fmt = **i;
                if(fmt.can_handle(filename))
                {
                    return fmt;
                }
            }
            throw exception("gfx::image(no format for '%s')", vfs::get_base_name(filename) );
        }
        
        
        
        surface * image::load_surface(const string &filename, const pixel_format fmt ) const
        {
            return get_format_for(filename).load_surface(filename, fmt);
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Loading
        //
        ////////////////////////////////////////////////////////////////////////
        
        surface * image::format:: load_surface(const string &filename, const pixel_format fmt ) const
        {
            const bitmap::pointer bmp( load(filename,fmt.depth, fmt.put_rgba_cb, &fmt) );
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
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Saving
        //
        ////////////////////////////////////////////////////////////////////////
        
        void image::format:: save_surface(const string  &filename,
                                          const surface &surf,
                                          const char    *options) const
        {
            save(filename, surf, surf.format.get_rgba_cb, &surf.format, options);
        }
        
        
        bool image::format::  can_handle(const string &filename) const throw()
        {
            const char *ext = vfs::get_extension(filename);
            if(!ext)
                return false;
            
            const size_t len = strlen(ext);
            const char **arr = extensions();
            assert(arr);
            
            while(*arr)
            {
                const char *txt = *arr;
                //std::cerr << "testing " << txt << std::endl;
                const char  num = strlen(txt);
                if(len==num)
                {
                    bool match = true;
                    for(size_t i=0;i<num;++i)
                    {
                        if( tolower(ext[i]) != tolower(txt[i]) )
                        {
                            match = false;
                            break;
                        }
                    }
                    if(match)
                    {
                        return true;
                    }
                }
                ++arr;
            }
            
            return false;
        }
        
        
        
    }
}
