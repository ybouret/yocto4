#include "yocto/aqueous/initializer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        constraint::~ constraint() throw() {}
        
        constraint:: constraint( const library &L, double v ) throw() :
        value(0), W(), lib(L) {}
        
        void constraint:: add( const string &id, double w )
        {
            if( ! lib.search(id) )
                throw exception("constraint( no '%s' )", id.c_str() );
            
            if( !W.insert(id, w) )
                throw exception("constraint( multiple '%s' )", id.c_str() );
        }
        
        
        initializer:: ~initializer() throw()
        {
        }
        
        initializer:: initializer( const library &L ) throw() :
        constraints(),
        lib(L)
        {
        }
        
        
        constraint & initializer:: create( double v )
        {
            constraint::ptr p( new constraint(lib,v) );
            constraints.push_back(p);
            return *p;
        }
        
    }
    
}
