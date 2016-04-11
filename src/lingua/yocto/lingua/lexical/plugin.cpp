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
            trigger(expr)
            {
                link_to(lxr);
            }

            void plugin:: hook( scanner &target )
            {
                target.call(name,trigger,this,&plugin::on_call);
            }


        }
    }
}
