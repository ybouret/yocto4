#include "yocto/chemical/parameters.hpp"

namespace yocto
{
    
    namespace chemical
    {
        parameters:: ~parameters() throw()
        {
            lib.decrease();
        }
        
        parameters:: parameters(const library &user_lib) :
        nvar(0),
        lib(user_lib),
        db(8,as_capacity)
        {
        }
        
        
    }
    
}
