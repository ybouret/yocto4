#include "yocto/gfx/surface.hpp"

namespace yocto
{
    namespace gfx
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // surface::line
        //
        ////////////////////////////////////////////////////////////////////////
        surface::line:: line( void *entry, const surface &s ) throw() :
        w(s.w),
        addr(entry),
        peek(s.pixels->peek)
        {
            assert(addr);
            assert(peek);
        }
        
        
        void *       surface:: line:: operator[](size_t x) throw()
        {
            assert(addr);
            assert(peek);
            assert(x<w);
            return peek(addr,unit_t(x));
        }
        
        const void * surface:: line:: operator[](size_t x) const throw()
        {
            assert(addr);
            assert(peek);
            assert(x<w);
            return peek((void*)addr,unit_t(x));
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // surface
        //
        ////////////////////////////////////////////////////////////////////////
        surface:: ~surface() throw()
        {
            assert(nline>0);
            assert(lines);
            memory::kind<memory::global>::release_as<line>(lines, nline);
        }
        
        surface:: surface( const color_format fmt, size_t width, size_t height) :
        format(fmt),
        pixels( new pixbuf(fmt.bytes_per_pixel,width,height) ),
        w(width),
        h(height),
        nline(h),
        lines( memory::kind<memory::global>::acquire_as<line>(nline) )
        {
            link(0,0);
        }
        
        
        void surface:: link( unit_t xoff, unit_t yoff ) throw()
        {
            const size_t s = pixels->stride;
            uint8_t     *p = static_cast<uint8_t *>( pixels->entry(xoff,yoff) );
            for(size_t i=0;i<h;++i,p+=s)
            {
                new (lines+i) line(p,*this);
            }
        }
        
        surface::line & surface:: operator[](size_t y) throw()
        {
            assert(y<h);
            return lines[y];
        }
        
        const surface::line & surface:: operator[](size_t y) const throw()
        {
            assert(y<h);
            return lines[y];
        }
        
        pixel_t surface:: __get( const void *addr) const throw()
        {
            assert(addr);
            assert(format.get_pixel);
            return format.get_pixel(addr);
        }

        
        pixel_t surface:: get_pixel( unit_t x, unit_t y ) const throw()
        {
            assert(x<w);
            assert(y<h);
            assert(format.get_pixel);
            return format.get_pixel( lines[y][x] );
        }

        void  surface:: __put( void *addr, pixel_t C) throw()
        {
            assert(addr);
            assert(format.put_pixel);
            format.put_pixel(addr,C);
        }
        
        void surface:: put_pixel(unit_t x, unit_t y, pixel_t C) throw()
        {
            assert(x<w);
            assert(y<h);
            assert(format.put_pixel);
            format.put_pixel(lines[y][x],C);
        }
        
        
    }
}
