#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax 
        {
            
            rule:: ~rule() throw() {}
            
            
            rule:: rule( const string &id ) : 
            label(id) 
            {}
            
            rule:: rule( const rule &other ) :
            label( other.label )
            {
            }
            
            
            
        }

    }

}
