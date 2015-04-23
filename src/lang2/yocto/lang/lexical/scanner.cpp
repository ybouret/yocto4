#include "yocto/lang/lexical/scanner.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            scanner:: ~scanner() throw() {}

            scanner:: scanner(const string &id ) :
            name(id),
            rules(),
            cache()
            {
            }
            
        }
    }

}
