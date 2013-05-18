#ifndef YOCTO_LINGUA_SYNTAX_AGGREGATE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_AGGREGATE_INCLUDED 1

#include "yocto/lingua/syntax/composite.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            class aggregate : public composite
            {
            public:
                virtual ~aggregate() throw();
                Y_SYNTAX_RULE_API();
                
                explicit aggregate( const string &id);
                
            private:
                YOCTO_DISABLE_COPY(aggregate);
            };
        }
        
    }
}

#endif
