#ifndef YOCTO_LINGUA_SYNTAX_JOKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lingua/syntax/rules.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
            
            class joker : public rule
            {
            public:
                virtual ~joker() throw();
                
            protected:
                explicit joker(const string &id, rule &r);
                
                rule::ptr handle;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };
            
            
            class optional : public joker
            {
            public:
                virtual ~optional() throw();
                explicit optional( const string &id, rule &r);
                
                Y_SYNTAX_RULE_API();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };
        }
        
    }
}


#endif

