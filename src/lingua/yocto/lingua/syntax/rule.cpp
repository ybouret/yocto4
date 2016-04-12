#include "yocto/lingua/syntax/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            rule:: ~rule() throw()
            {}


            rule:: rule( const uint32_t t) throw() :
            next(0), prev(0), uuid(t), self(0)
            {
            }
            
        }
    }
}
