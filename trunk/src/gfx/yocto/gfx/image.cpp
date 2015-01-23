#include "yocto/gfx/image.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace gfx
    {
        
        image::format:: format(const char *id) : name(id)
        {
        }
        
        image::format:: ~format() throw()
        {
        }
        
        const string & image::format:: key() const throw()
        {
            return name;
        }
        
        
    }
}

#include "yocto/fs/vfs.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {
        image:: image() : formats(8,as_capacity)
        {
        }
        
        image:: ~image() throw()
        {
        }
        
        const char image::name[] = "image";
        
        void image:: declare( format *fmt )
        {
            assert(fmt);
            format::pointer p(fmt);
            if(!formats.insert(p))
            {
                throw exception("multiple image format '%s'", fmt->name.c_str());
            }
        }
        
        const image::format & image:: operator[](const string &id) const
        {
            const format::pointer *pp = formats.search(id);
            if(!pp)
                throw exception("no image format '%s'", id.c_str());
            return **pp;
        }
        
        const image::format & image:: operator[](const char *id) const
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        const image::format & image::get_format_for( const string &path ) const
        {
            string ext = vfs::get_extension(path);
            ext.to_lower();
            for( image::format::db::const_iterator i= formats.begin();i != formats.end();++i)
            {
                const format &fmt = **i;
                const char  **exts = fmt.extensions();
                for(size_t j=0;;++j)
                {
                    const char *tmp = exts[j];
                    if(!tmp)
                    {
                        break;
                    }
                    if( 0 == strcmp(tmp,ext.c_str()))
                    {
                        return fmt;
                    }
                }
            }
            throw exception("no format may load '%s'", ext.c_str());
        }
        
        bitmap *image::load(const string         &path,
                            unit_t                depth,
                            image::put_rgba_proc  proc,
                            const void           *args,
                            const char           *options) const
        {
            const format &fmt = get_format_for(path);
            return fmt.load(path, depth, proc, args, options);
        }
        
    }
    
}
