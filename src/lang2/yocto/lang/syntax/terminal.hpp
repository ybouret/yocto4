#ifndef YOCTO_LANG_SYNTAX_TERM_INCLUDED
#define YOCTO_LANG_SYNTAX_TERM_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class terminal : public rule
            {
            public:
                explicit terminal(const string &id);
                virtual ~terminal() throw();

                //! accept if lexeme->label == this->label
                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS );


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
            };

        }
    }
}

#endif
