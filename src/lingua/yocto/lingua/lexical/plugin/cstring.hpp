#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_CSTRING_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_CSTRING_INCLUDED 1

#include "yocto/lingua/lexical/plugin/_string.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            class cstring : public _string
            {
            public:
                virtual ~cstring() throw();
                explicit cstring(const string &id,
                                 lexer        &lxr);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(cstring);

            };

        }

    }

}

#endif
