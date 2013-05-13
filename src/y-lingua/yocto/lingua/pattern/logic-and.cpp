#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // AND
        //
        ////////////////////////////////////////////////////////////////////////
        AND::  AND() throw() {}
        AND:: ~AND() throw() {}
        
        AND:: AND( const AND &other ) : logical(other) {}
        
        AND * AND::create() { return new AND(); }
        
        void AND:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        pattern * AND::clone() const { return new AND( *this ); }
        
        bool AND:: accept( source &src )
        {
            assert(0==size);
            for(pattern *p=operands.head;p;p=p->next)
            {
                assert(0==p->size);
                if(! p->accept(src) )
                {
                    assert(0==p->size);
                    src.unget( *this );
                    return false;
                }
                merge_back(*p);
            }
            
            return true;
        }

    }
    
}
