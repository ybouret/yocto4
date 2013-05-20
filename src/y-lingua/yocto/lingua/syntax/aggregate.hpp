#ifndef YOCTO_LINGUA_SYNTAX_AGGREGATE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_AGGREGATE_INCLUDED 1

#include "yocto/lingua/syntax/composite.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            //! match a sequence of consecutive rules
            class aggregate : public composite
            {
            public:
                virtual ~aggregate() throw();
                Y_SYNTAX_RULE_API();
                
                explicit aggregate(const string &id, node_property ppty);
                const node_property behavior;

                inline aggregate & operator+=( rule &r ) { add(r); return *this; }

            private:
                YOCTO_DISABLE_COPY(aggregate);
            };
        }
        
    }
}

#endif
