#ifndef YOCTO_LINGUA_SYNTAX_RULE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_RULE_INCLUDED 1

#include "yocto/lingua/lexer.hpp"
#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


#define YOCTO_LINGUA_SYNTAX_RULE_IS(DERIVED) do { assert(0==self); DERIVED *addr = this; self = addr; } while(false)

            //! a rule to accept the output of a lexer
            class rule : public object
            {
            public:
                rule          *next;
                rule          *prev;
                const string   label;
                const uint32_t uuid;
                uint32_t       flag;
                void          *self;

                virtual ~rule() throw();
                
            protected:
                explicit rule(const string &id,const uint32_t t);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

            typedef core::list_of_cpp<rule> r_list;
        }
    }
}

#endif

