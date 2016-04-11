#include "yocto/lingua/lexer.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            plugin:: ~plugin() throw()
            {
                
            }


            plugin:: plugin(const string &id,
                            lexer        &lxr,
                            const string &expr) :
            scanner(id,lxr.line),
            motif( regexp(expr,&lxr.dict) )
            {
                link_to(lxr);
            }

            
        }
    }
}
