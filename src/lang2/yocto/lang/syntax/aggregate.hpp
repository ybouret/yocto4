#ifndef YOCTO_LANG_SYNTAX_AGG_INCLUDED
#define YOCTO_LANG_SYNTAX_AGG_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/logic.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class aggregate : public logical
            {
            public:
                virtual ~aggregate() throw();
                explicit aggregate(const string &id);

                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(aggregate);
            };
        }

    }

}

#endif
