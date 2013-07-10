#include "yocto/gfx/surface.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
    namespace gfx
    {
        surface:: ~surface() throw()
        {
            memory::kind<memory::global>::release_as<row>(rows, nrow);
        }
        
        
        static inline
        void * move8(void *p, size_t x) throw()
        {
            return static_cast<uint8_t*>(p)+x;
        }
        
        static inline
        void * move16(void *p,size_t x) throw()
        {
            return static_cast<uint16_t*>(p)+x;
        }
        
        static inline
        void * move24(void *p,size_t x) throw()
        {
            return static_cast<uint8_t*>(p)+3*x;
        }
        
        static inline
        void * move32(void *p,size_t x) throw()
        {
            return static_cast<uint32_t*>(p)+x;
        }
        
        
        
        surface:: row:: row( size_t width, void *start, const size_t BPP) throw() :
        w(width),
        p(start),
        move(0)
        {
            assert(w>0);
            assert(p);
            switch( BPP )
            {
                case 1:
                    move = move8;
                    break;
                    
                    
                case 2:
                    move = move16;
                    break;
                    
                case 3:
                    move = move24;
                    break;
                    
                case 4:
                    move = move32;
                    break;
            }
        }
        
        
        void * surface::row:: operator[](size_t i) throw()
        {
            assert(i<w);
            assert(move);
            return move(p,i);
        }
        
        const void * surface::row:: operator[](size_t i) const throw()
        {
            assert(i<w);
            assert(move);
            return move((void*)p,i);
        }
        
        
        surface:: surface( const color_format &fmt, size_t width, size_t height ) :
        format( fmt ),
        pixels( new pixbuf(fmt.bytes_per_pixel,width,height) ),
        w(pixels->w),
        h(pixels->h),
        nrow(h),
        rows( memory::kind<memory::global>::acquire_as<row>(nrow) )
        {
            link(0,0);
        }
        
        void surface:: link( unit_t xoff, unit_t yoff ) throw()
        {
            uint8_t *p = (uint8_t*)(pixels->get(xoff, yoff));
            for(size_t j=0;j<h;++j)
            {
                new (rows+j) row(w,p,pixels->bytes_per_pixel);
                p += pixels->stride;
            }
        }
        
        surface::row & surface:: operator[]( size_t j ) throw()
        {
            assert(j<h);
            return rows[j];
        }
        
        const surface::row & surface:: operator[]( size_t j ) const throw()
        {
            assert(j<h);
            return rows[j];
        }
        
               
    }
}
