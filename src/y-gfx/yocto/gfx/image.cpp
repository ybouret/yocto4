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
        
        ////////////////////////////////////////////////////////////////////////
        //
        // image format
        //
        ////////////////////////////////////////////////////////////////////////
        image::format::format( const char *id ) :
        name(id)
        {
        }
        
        image:: format:: ~format() throw()
        {
        }
        
        const string & image::format::key() const throw() { return name; }
        
    }
}

