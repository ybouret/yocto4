#include "yocto/lang/syntax/grammar.hpp"

namespace yocto
{
    namespace lang
    {

        namespace syntax
        {


            grammar:: ~grammar() throw()
            {
            }


            grammar:: grammar(const string &id) :
            name(id),
            rules()
            {

            }

            void grammar:: set_root( rule &r ) throw()
            {
                assert( rules.owns(&r) );
                rules.move_to_front(&r);
            }


        }

    }

}
