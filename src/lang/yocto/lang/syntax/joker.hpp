#ifndef YOCTO_LANG_SYNTAX_JOKER_INCLUDED
#define YOCTO_LANG_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lang/lexer.hpp"
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

                virtual void *content() const throw(); //!< return &jk;


            protected:
                explicit joker(const string &id, rule &r, uint32_t uu);
                
                const rule *jk;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };
            
        }
    }
}

#endif
