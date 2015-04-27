#ifndef YOCTO_LANG_SYNTAX_LOGIC_INCLUDED
#define YOCTO_LANG_SYNTAX_LOGIC_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/rule.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class logical : public rule
            {
            public:
                typedef addr_node<rule> operand;
                
                virtual ~logical() throw();

                void append( rule &r );
                logical & operator<<( rule &r );

            protected:
                explicit logical(const string &id);
                addr_list<rule>  operands;

                YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
            };


        }

    }

}

#endif

