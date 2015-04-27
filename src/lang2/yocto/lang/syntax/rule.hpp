#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

#define Y_LANG_SYNTAX_RULE_ACCEPT_ARGS xnode * &tree, lexer &lxr, source &src, ios::istream &fp
            
            class rule : public object
            {
            public:
                virtual ~rule() throw();
                const string &label;


                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) = 0;

            protected:
                explicit rule(const string &id);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

        }
    }
}

#endif
