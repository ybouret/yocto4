#ifndef YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            class walker : public object
            {
            public:
                virtual ~walker() throw();

            protected:
                explicit walker() throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(walker);
            };
        }
    }
}

#endif
