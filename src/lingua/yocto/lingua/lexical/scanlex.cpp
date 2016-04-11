#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            void scanner:: link_to(lexer &parent) throw()
            {
                assert(0==lex);
                assert(0==dict);
                assert(&line == & (parent.line) );
                lex  = &parent;
                dict = &parent.dict;
            }


        }

    }

}

