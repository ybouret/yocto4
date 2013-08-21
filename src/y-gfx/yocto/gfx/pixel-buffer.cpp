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
        
        static inline
        void * __move1(void *addr, unit_t n)
        {
            return static_cast<uint8_t*>(addr)+n;
        }
        
        static inline
        void * __move2(void *addr, unit_t n)
        {
            return static_cast<uint16_t*>(addr)+n;
        }
        
        static inline
        void * __move3(void *addr, unit_t n)
        {
            return static_cast<uint8_t*>(addr)+(n*3);
        }
        
        static inline
        void * __move4(void *addr, unit_t n)
        {
            return static_cast<uint32_t*>(addr)+n;
        }
        
        pixbuf:: pixbuf( size_t BytesPerPixel, size_t Width, size_t Height) :
        metrics(BytesPerPixel),
        counted(),
        w( __check_width(  Width ) ),
        h( __check_height( Height) ),
        bytes_per_line( w * bytes_per_pixel ),
        stride( bytes_per_line     ),
        move(0),
        bytes(  bytes_per_line * h ),
        entry(  memory::kind<memory::global>::acquire(bytes) ),
        allocated(true)
        {
            switch(bytes_per_pixel)
            {
                case 1:
                    move = __move1;
                    break;
                    
                case 2:
                    move = __move2;
                    break;
                    
                case 3:
                    move = __move3;
                    break;
                    
                case 4:
                    move = __move4;
                    break;
                    
            }
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
