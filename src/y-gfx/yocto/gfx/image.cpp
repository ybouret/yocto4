#include "yocto/gfx/image.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // image
        //
        ////////////////////////////////////////////////////////////////////////

        const char image::name[] = "gfx::image";
        
        image:: image()
        {
        }
        
        image:: ~image() throw()
        {
            
        }
        
        
        void image:: operator()( format *fmt )
        {
            const fmt_ptr p(fmt);
            
            if( !formats.insert(p) )
                throw exception("multiple image format '%s'", p->name.c_str());
            
        }
        
        size_t image:: size()  const throw()
        {
            return formats.size();
        }
        
        image::iterator image::begin() const throw()
        {
            return formats.begin();
        }
        
        image::iterator image::end() const throw()
        {
            return formats.end();
        }
        
        const image::format & image:: operator[](const string &id) const
        {
            const fmt_ptr *ppFmt = formats.search(id);
            if( !ppFmt )
                throw exception("unregistered image format '%s'", id.c_str());
            
            return **ppFmt;
        }
        
        bool image:: has( const string &id) const throw()
        {
            return 0 != formats.search(id);
        }

        bool image:: has( const char *tgt ) const
        {
            const string id(tgt);
            return 0 != formats.search(id);
        }

        
        image::format::format( const char *id ) :
        name(id)
        {
        }
        
        image::format:: ~format() throw()
        {
        }
        
        const string & image:: format::key() const throw() { return name; }
        
    }
}

