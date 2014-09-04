#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
            class rule : public object
            {
            public:
                const string label;
                rule        *next;
                rule        *prev;
                
                virtual ~rule() throw();
                
            protected:
                explicit rule(const string &id );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };
            
            typedef core::list_of_cpp<rule> r_list;
            
        }
    }
}

#endif

