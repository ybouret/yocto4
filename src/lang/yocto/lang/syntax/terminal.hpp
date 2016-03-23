#ifndef YOCTO_LANG_SYNTAX_TERM_INCLUDED
#define YOCTO_LANG_SYNTAX_TERM_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            //! a terminal rule: lexeme matches label !
            class terminal : public rule
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('T','E','R','M');
                
                const property modifier; //!< for xnode
                explicit terminal(const string &id,const property ppty=standard);
                virtual ~terminal() throw();

                //! accept if lexeme->label == this->label
                virtual bool accept( Y_LANG_SYNTAX_RULE_ACCEPT_ARGS ) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;
                virtual void cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const;

                virtual void *content() throw(); //!< &modifier

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
            };

        }
    }
}

#endif
