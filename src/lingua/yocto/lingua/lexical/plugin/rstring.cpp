#include "yocto/lingua/lexical/plugin/rstring.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            rstring:: ~rstring() throw()
            {
            }


            rstring:: rstring(const string &id, lexer &lxr) :
            _string(id,lxr,"'")
            {
                
            }
            
        }
    }
}
