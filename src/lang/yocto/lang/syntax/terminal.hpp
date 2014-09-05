#ifndef YOCTO_LANG_SYNTAX_TERMINAL_INCLUDED
#define YOCTO_LANG_SYNTAX_TERMINAL_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            //! match a lexeme with the SAME label
            class terminal : public rule
            {
            public:
                virtual ~terminal() throw();
                explicit terminal(const string &id, const property ppty);
                const property semantic;
                
                YOCTO_LANG_SYNTAX_RULE_MATCH_DECL();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
            };
            
        }
        
    }
}

#endif
