#ifndef YOCTO_LANG_SYNTAX_OPT_INCLUDED
#define YOCTO_LANG_SYNTAX_OPT_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/joker.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class optional : public joker
            {
            public:
                explicit optional(const string &label, rule &r );
                virtual ~optional() throw();

                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };

        }

    }

}
#endif