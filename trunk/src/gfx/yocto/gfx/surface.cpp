#include "yocto/gfx/surface.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gfx
    {
        
        surface::row:: row( void *addr, const surface *surf ) throw() :
        p(addr),
        s(surf)
        {
            assert(p!=0);
            assert(s!=0);
        }
        
        void * surface::row:: operator[](unit_t i) throw()
        {
            assert(i>=0);
            assert(i<s->w);
            return s->peek(p,i);
        }
        
        const void * surface::row:: operator[](unit_t i) const throw()
        {
            assert(i>=0);
            assert(i<s->w);
            return s->peek(p,i);
        }
                
        
        surface:: ~surface() throw()
        {
            kill_rows();
        }
        
        surface:: surface( const pixel_format fmt, unit_t width, unit_t height) :
        bitmap( fmt.depth, width, height ),
        format( fmt ),
        nrow(0),
        rows(0)
        {
            make_rows();
        }
        
        surface:: surface( const pixel_format fmt, const bitmap::pointer &bmp, const rectangle *rect ) :
        bitmap( bmp, rect),
        format( fmt ),
        nrow(0),
        rows(0)
        {
            check_depths("surface", fmt.depth, "bitmap", this->d);
            make_rows();
        }
        
        void surface:: kill_rows() throw()
        {
            memory::kind<memory::global>::release_as<row>(rows, nrow);
        }
        
        void surface:: make_rows()
        {
            nrow = h;
            rows = memory::kind<memory::global>::acquire_as<row>(nrow);
            for(unit_t i=0;i<h;++i)
            {
                new (rows+i) row( get_line(i), this);
            }
        }
        
        
        surface::row & surface::operator[](unit_t j) throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows[j];
        }
        
        const surface::row & surface::operator[](unit_t j) const throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows[j];
        }
       
        void surface:: put_pixel(void *addr, const pixel_t C)
        {
            format.put_pixel(addr,C);
        }
        
        void surface:: put_pixel(void *addr, const rgb_t &C)
        {
            format.put_pixel(addr,format.map_rgb(C));
        }
        
        void surface:: put_pixel(void *addr, const rgba_t &C)
        {
            format.put_pixel(addr,format.map_rgba(C));
        }

        pixel_t surface:: get_pixel(const void *addr) const
        {
            return format.get_pixel(addr);
        }
        
        rgb_t surface::get_rgb(const void *addr) const
        {
            return format.get_rgb( format.get_pixel(addr) );
        }

        rgba_t surface::get_rgba(const void *addr) const
        {
            return format.get_rgba( format.get_pixel(addr) );
        }
        
    

        
    }
}
