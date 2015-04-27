#ifndef YOCTO_LANG_SYNTAX_COUNTING_INCLUDED
#define YOCTO_LANG_SYNTAX_COUNTING_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/joker.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            class counting : public logical
            {
            public:
                virtual ~counting() throw();
                explicit counting(size_t n);
                
                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                
                const size_t nmin;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
                static  unsigned counter;
                static  string   make_label();
            };
        }
        
    }
    
}

#endif
