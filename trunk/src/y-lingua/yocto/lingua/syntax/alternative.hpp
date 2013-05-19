#ifndef YOCTO_LINGUA_SYNTAX_ALTERNATIVE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_ALTERNATIVE_INCLUDED 1

#include "yocto/lingua/syntax/composite.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            //! match a of possible rules
            class alternative : public composite
            {
            public:
                virtual ~alternative() throw();
                Y_SYNTAX_RULE_API();
                
                explicit alternative(const string &id);
                
            private:
                YOCTO_DISABLE_COPY(alternative);
            };
        }
        
    }
}

#endif
