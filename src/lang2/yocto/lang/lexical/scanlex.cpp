#include "yocto/lang/lexer.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {


            void scanner:: link_to(lexer &parent)
            {
                if(lex)
                {
                    throw exception("<%s> already linked to [%s]", name.c_str(), lex->name.c_str());
                }

                lex = &parent;
            }
            
        }
        
    }
    
}


namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            
        }

    }

}
