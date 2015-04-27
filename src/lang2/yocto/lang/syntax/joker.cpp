#include "yocto/lang/syntax/joker.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            joker:: ~joker() throw()
            {
            }

            joker:: joker(const string &id, rule &r) :
            rule(id),
            jk(r)
            {
                
            }
        }

    }

}
