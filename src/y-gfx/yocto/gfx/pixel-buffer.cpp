#include "yocto/gfx/pixel-buffer.hpp"
#include "yocto/exception.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
    namespace gfx
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // pixbuf
        //
        ////////////////////////////////////////////////////////////////////////

        pixbuf:: ~pixbuf() throw()
        {
            assert(entry);
            if(allocated)
            {
                assert(bytes);
                memory::kind<memory::global>::release(entry, bytes);
            }
        }
        
        static inline
        size_t __check_width(size_t w)
        {
            if(w<=0)
                throw exception("gfx::pixbuf(no width)");
            return w;
        }
        
        static inline
        size_t __check_height(size_t h)
        {
            if(h<=0)
                throw exception("gfx::pixbuf(no height)");
            return h;
        }
        
        pixbuf:: pixbuf( size_t BytesPerPixel, size_t Width, size_t Height) :
        metrics(BytesPerPixel),
        counted(),
        w( __check_width(  Width ) ),
        h( __check_height( Height) ),
        bytes_per_line( w * bytes_per_pixel ),
        stride( bytes_per_line     ),
        bytes(  bytes_per_line * h ),
        entry(  memory::kind<memory::global>::acquire(bytes) ),
        allocated(true)
        {
            
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // pixbuf::pointer
        //
        ////////////////////////////////////////////////////////////////////////
        pixbuf::pointer:: pointer( pixbuf *p ) throw():
        host(p)
        {
            assert(host);
            host->withhold();
        }
        
        pixbuf:: pointer:: ~pointer() throw()
        {
            assert(host);
            if( host->liberate() )
            {
                delete host;
                host = 0;
            }
        }
        
        pixbuf:: pointer:: pointer( const pointer &other ) throw() :
        host( other.host )
        {
            assert(host);
            host->withhold();
        }
        
        pixbuf *  pixbuf:: pointer::operator->() throw()
        {
            assert(host);
            return host;
        }

        const pixbuf *  pixbuf:: pointer::operator->() const throw()
        {
            assert(host);
            return host;
        }
        
    }
}
