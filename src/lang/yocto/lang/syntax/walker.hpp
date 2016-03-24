#ifndef YOCTO_LANG_SYNTAX_WALKER_INCLUDED
#define YOCTO_LANG_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
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
