#ifndef YOCTO_LANG_SYNTAX_ALT_INCLUDED
#define YOCTO_LANG_SYNTAX_ALT_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/logic.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class alternate : public logical
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('[', '|', '|', ']');
                virtual ~alternate() throw();
                explicit alternate();

                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                virtual void viz( ios::ostream &fp ) const;
                virtual void lnk( ios::ostream &fp ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(alternate);
                static  unsigned counter;
                static  string   make_label();
            };
        }
        
    }
    
}

#endif
