#include "yocto/lang/syntax/logic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            logical:: ~logical() throw() {}

            logical:: logical(const string &id, uint32_t uu) :
            rule(id,uu), operands()
            {

            }

            void logical:: add( rule &r )
            {
                append( &r );
            }


            logical & logical:: operator<<( rule &r )
            {
                add(r);
                return *this;
            }

            void * logical::content() const throw()
            {
                const operands &ops = *this;
                return (void*)&ops;
            }
        }

    }

}

