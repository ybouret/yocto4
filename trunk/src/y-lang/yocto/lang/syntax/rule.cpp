#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax 
        {
            
            rule:: ~rule() throw() {}
            
            
            rule:: rule( const string &id ) : 
            name(id) 
            {}
            
            rule:: rule( const rule &other ) :
            name( other.name )
            {
            }
            
            
            
        }

    }

}
