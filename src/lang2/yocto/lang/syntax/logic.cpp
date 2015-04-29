#include "yocto/lang/syntax/logic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            logical:: ~logical() throw() {}

            logical:: logical(const string &id, uint32_t uu) :
            rule(id,uu)
            {

            }

            void logical:: append( rule &r )
            {
                operands.append( &r );
            }


            logical & logical:: operator<<( rule &r )
            {
                append(r);
                return *this;
            }
        }

    }

}

