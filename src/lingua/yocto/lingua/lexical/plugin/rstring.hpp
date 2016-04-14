#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_RSTRING_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_RSTRING_INCLUDED 1

#include "yocto/lingua/lexical/plugin/_string.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            class rstring : public _string
            {
            public:
                virtual ~rstring() throw();
                explicit rstring(const string &id,
                                 lexer        &lxr);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rstring);

            };
            
        }
        
    }
    
}

#endif
