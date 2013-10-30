#include "yocto/gfx/surface.hpp"

namespace yocto
{
    namespace gfx
    {
        
        surface:: row:: row( void *addr, unit_t W) throw() :
        w(W),
        p(addr)
        {
            
        }
        
        
        surface * surface:: create(const format fmt, unit_t W, unit_t H)
        {
            return new surface(fmt,W,H);
        }
        
        surface:: surface( const format &fmt, unit_t W, unit_t H ) :
        format(fmt),
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
            nrow = h; assert(nrow>0);
            rows = memory::kind<memory::global>::acquire_as<row>(nrow);
            uint8_t *p = static_cast<uint8_t *>(entry);
            for(unit_t i=0;i<h;++i,p+=stride)
                new (rows+i) row(p,w);
        }
        
        
    }
    
}
