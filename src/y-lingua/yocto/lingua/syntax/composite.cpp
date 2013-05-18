#include "yocto/lingua/syntax/composite.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
            
            composite:: ~composite() throw() {}
            
            
            composite:: composite( const string &id ) :
            rule(id),
            items(4,as_capacity)
            {
            }
            
            

        }

    }

}
