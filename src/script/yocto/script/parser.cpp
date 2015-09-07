#include "yocto/script/parser.hpp"
#include "yocto/lang/generator.hpp"

namespace yocto
{
    namespace Script
    {

        static const char ScriptGrammar[] =
        {
#include "script.inc"
        };


        Parser:: Parser() :
        impl( lang::parser::generate("Script",ScriptGrammar,sizeof(ScriptGrammar),true) )
        {
            
        }

        Parser:: ~Parser() throw()
        {
            delete impl;
        }


        Node * Parser:: getAST( ios::istream &input )
        {
            return impl->run(input);
        }

    }
}
