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
        
		image:: image() : formats(8,as_capacity)
        {
        }
        
        image:: ~image() throw()
        {
            
        }
        
        
        void image:: operator()( format *fmt )
        {
            const format::pointer p(fmt);
            
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
            const format::pointer *ppFmt = formats.search(id);
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
        
        ////////////////////////////////////////////////////////////////////////
        //
        // image::format
        //
        ////////////////////////////////////////////////////////////////////////
        image::format::format( const char *id ) :
        name(id)
        {
        }
        
        image::format:: ~format() throw()
        {
        }
        
        const string & image:: format::key() const throw() { return name; }
        
        void image::format::save_surface(const string  &filename,
                                         const surface &surf,
                                         const char    *options) const
        {
            addr2rgba proc( &surf, & surface::to_rgba);
            save(filename,surf,proc,options);
        }
        
    }
}

