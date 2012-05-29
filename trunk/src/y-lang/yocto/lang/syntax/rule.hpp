#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/lexer.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax 
        {
            
            class rule : public object 
            {
            public:
                const string name;
                virtual ~rule() throw();
                
                virtual rule *clone() const = 0;
                virtual bool  accept( lexer &lx ) = 0;
                
                
                
            protected:
                explicit rule( const string &id );
                rule( const rule & );
                
            private:
                YOCTO_DISABLE_ASSIGN(rule);
            };
            
        }
    }
}


#endif
