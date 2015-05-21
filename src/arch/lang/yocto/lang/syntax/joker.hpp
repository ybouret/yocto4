#ifndef YOCTO_LANG_SYNTAX_JOKER_INCLUDED
#define YOCTO_LANG_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            class joker : public rule
            {
            public:
                virtual ~joker() throw();

                
                
            protected:
                explicit joker(const string &id, rule &other);
                rule &sub;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };
            
            
            class optional : public joker
            {
            public:
                virtual ~optional() throw();
                explicit optional(const string &id, rule &other);

                
                YOCTO_LANG_SYNTAX_RULE_MATCH_DECL();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };
            
            class at_least : public joker
            {
            public:
                virtual ~at_least() throw();
                explicit at_least(const string &id, rule &other, size_t count);
                
                const size_t value;
                
                YOCTO_LANG_SYNTAX_RULE_MATCH_DECL();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(at_least);
            };
            
            
            
        }

    }

}

#endif
