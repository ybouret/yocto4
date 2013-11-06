#include "yocto/gfx/surface.hpp"


namespace yocto
{
    namespace gfx
    {
        
        surface:: row:: row( void *data, unit_t W, peek_proc proc) throw() :
        w(W),
        addr(data),
        peek(proc)
        {
            assert(addr!=0);
			assert(w>0);
			assert(peek!=0);
        }
        
        void * surface::row:: operator[](unit_t x) throw()
        {
            assert(x>=0);
            assert(x<w);
            assert(addr);
            assert(peek);
            return peek(addr,x);
        }
        
        const void * surface::row:: operator[](unit_t x) const throw()
        {
            assert(x>=0);
            assert(x<w);
            assert(addr);
            assert(peek);
            return peek(addr,x);
        }


        
        
        surface * surface:: create(const pixel_format fmt, unit_t W, unit_t H)
        {
            return new surface(fmt,W,H);
        }
        
        surface:: surface( const pixel_format &fmt, unit_t W, unit_t H ) :
        pixel_format(fmt),
        bitmap(fmt.bytes,W,H),
        rows(0),
        nrow(0)
        {
            create_rows();
        }
        
        surface:: ~surface() throw()
        {
            delete_rows();
        }
        
        
        void surface:: delete_rows() throw()
        {
            memory::kind<memory::global>::release_as<row>(rows, nrow);
        }
        
        void surface:: create_rows()
        {
			assert(entry!=0);
			assert(h>0);
			assert(w>0);
            nrow = h; assert(nrow>0);
            rows = memory::kind<memory::global>::acquire_as<row>(nrow);
            uint8_t *p = static_cast<uint8_t *>(entry);
            for(unit_t i=0;i<h;++i,p+=stride)
                new ( &rows[i] ) row(p,w,peek);
        }
        
        
        surface::row & surface::operator[]( unit_t y ) throw()
        {
			assert(rows);
            assert(y>=0);
            assert(y<h);
            return rows[y];
        }
        
        const surface::row & surface::operator[]( unit_t y ) const throw()
        {
			assert(rows);
            assert(y>=0);
            assert(y<h);
            return rows[y];
        }

        rgb_t surface:: to_rgba(const void *addr) const throw()
        {
			assert(get_pixel);
            assert(addr);
            return get_rgba(get_pixel(addr) );
        }

        
    }
    
}
