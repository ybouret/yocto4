#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // OR
        //
        ////////////////////////////////////////////////////////////////////////
        OR::  OR() throw() : logical(type) {}
        OR:: ~OR() throw() {}
        
        OR:: OR( const OR &other ) : logical(other) {}
        
        OR * OR::create() { return new OR(); }
        
        void OR:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        pattern * OR::clone() const { return new OR( *this ); }
        
        
        bool OR:: accept( source &src )
        {
            assert(0==size);
            for(pattern *p=operands.head;p;p=p->next)
            {
                assert(0==p->size);
                if( p->accept(src) )
                {
                    p->swap_with( *this );
                    assert(0==p->size);
                    return true;
                }
            }
            return false;
        }

    }
    
}
