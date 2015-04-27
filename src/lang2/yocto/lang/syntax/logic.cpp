#include "yocto/lang/syntax/logic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            logical:: ~logical() throw() {}

            logical:: logical(const string &id) :
            rule(id)
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

