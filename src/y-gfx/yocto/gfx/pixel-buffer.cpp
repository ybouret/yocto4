#include "yocto/gfx/pixel-buffer.hpp"
#include "yocto/exception.hpp"

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
            assert(entry_);
            if(allocated)
            {
                assert(bytes_);
                memory::kind<memory::global>::release(entry_, bytes_);
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
        peek(0),
        bytes_(  bytes_per_line * h ),
        entry_(  memory::kind<memory::global>::acquire(bytes_) ),
        allocated(true)
        {
            switch(bytes_per_pixel)
            {
                case 1:
                    (peek_proc &)peek = __move1;
                    break;
                    
                case 2:
                    (peek_proc &)peek = __move2;
                    break;
                    
                case 3:
                    (peek_proc &)peek = __move3;
                    break;
                    
                case 4:
                    (peek_proc &)peek = __move4;
                    break;
                    
            }
        }

        
        size_t      pixbuf:: bytes() const throw()
        {
            return bytes_;
        }
        
        void       * pixbuf:: entry( unit_t x, unit_t y ) throw()
        {
            assert(entry_);
            assert(peek);
            return peek(static_cast<uint8_t*>(entry_) + (y*stride),x);
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
