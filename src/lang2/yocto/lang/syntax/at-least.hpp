#ifndef YOCTO_LANG_SYNTAX_AT_LEAST_INCLUDED
#define YOCTO_LANG_SYNTAX_AT_LEAST_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/joker.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            class at_least : public joker
            {
            public:
                virtual ~at_least() throw();
                explicit at_least(rule &r, size_t n);
                
                virtual bool accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const;
                
                const size_t nmin;
                                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(at_least);
                static  unsigned counter;
                static  string   make_label(size_t);
            };
            

        }
        
    }
    
}

#endif
