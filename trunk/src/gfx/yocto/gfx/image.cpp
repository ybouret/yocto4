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

        
    }
}
