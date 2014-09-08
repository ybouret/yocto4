#ifndef YOCTO_LANG_LEXICAL_PLUGIN_CSTRING_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_CSTRING_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            class cstring : public plugin
            {
            public:
                virtual ~cstring() throw();
                explicit cstring(const string &id, lexer &parent);
                
                virtual pattern *trigger() const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(cstring);
            };
            
        }
        
    }
    
}

#endif
