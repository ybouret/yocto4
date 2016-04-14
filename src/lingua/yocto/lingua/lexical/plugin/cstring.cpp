#include "yocto/lingua/lexical/plugin/cstring.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            cstring:: ~cstring() throw()
            {
            }


            cstring:: cstring(const string &id, lexer &lxr) :
            _string(id,lxr,"\"")
            {


            }

        }
    }
}
