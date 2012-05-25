#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        lexer:: ~lexer() throw()
        {
        }
        
        lexer:: lexer( const string &main_id ) :
        line(0),
        scanners(4,as_capacity)
        {
            (void) declare( main_id );
            
        }
        
        lexical::scanner & lexer:: declare( const string &id )
        {
            lexical::scanner::ptr p( new lexical::scanner( id, line ) );
            if( ! scanners.insert(p) )
                throw exception("lexer(multiple scanner '%s')", id.c_str() );
            return *p;
        }
        
        
    }

}
