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


            class logical : public rule, public addr_list<rule>
            {
            public:
                typedef addr_node<rule> operand;
                
                virtual ~logical() throw();

                void add( rule &r );

                logical & operator<<( rule &r );


            protected:
                explicit logical(const string &id, uint32_t uu);

                YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
            };


        }

    }

}

#endif

