#include "yocto/lingua/syntax/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            rule:: ~rule() throw()
            {}


            rule:: rule(const string   &id,
                        const uint32_t  t)  :
            next(0),
            prev(0),
            label(id),
            flags(0),
            uuid(t),
            self(0)
            {
            }
            
        }
    }
}
