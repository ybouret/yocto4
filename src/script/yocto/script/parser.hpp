#ifndef YOCTO_SCRIPT_PARSER_INCLUDED
#define YOCTO_SCRIPT_PARSER_INCLUDED 1

#include "yocto/script/types.hpp"
#include "yocto/lang/parser.hpp"

namespace yocto
{
    namespace Script
    {
        typedef lang::syntax::xnode Node;
        
        class Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();

            Node *getAST( ios::istream &input);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Parser);
            lang::parser *impl;            
        };
    }
}

#endif
