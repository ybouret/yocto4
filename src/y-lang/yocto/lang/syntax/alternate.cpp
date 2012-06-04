#include "yocto/lang/syntax/alternate.hpp"

namespace yocto 
{
    namespace lang
    {
        namespace syntax 
        {
            
            alternate:: ~alternate() throw() {}
            
            alternate:: alternate( const string &id ) : compound(id) 
            {
            }
            
            alternate:: alternate( const alternate &other ) :
            compound( other )
            {
            }
            
            
            rule * alternate:: clone() const { return new alternate( *this ); }
            
        }
    }
    
}
