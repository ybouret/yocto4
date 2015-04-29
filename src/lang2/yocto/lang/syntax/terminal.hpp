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
                const property modifier; //!< for xnode
                explicit terminal(const string &id,const property ppty=standard);
                virtual ~terminal() throw();

                //! accept if lexeme->label == this->label
                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
            };

        }
    }
}

#endif
