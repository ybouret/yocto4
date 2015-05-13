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

            joker:: joker(const string &id, rule &r, uint32_t uu) :
            rule(id,uu),
            jk(r)
            {
                
            }

            void * joker:: content() throw()
            {
                return (void*)&jk;
            }

        }

    }

}
