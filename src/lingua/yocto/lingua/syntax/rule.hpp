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

            //! a rule to accept the output of a lexer
            class rule : public object
            {
            public:
                rule          *next;
                rule          *prev;
                const uint32_t uuid;
                void          *self;

                virtual ~rule() throw();
                
            protected:
                explicit rule(const uint32_t t) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

            typedef core::list_of_cpp<rule> r_list;
        }
    }
}

#endif

