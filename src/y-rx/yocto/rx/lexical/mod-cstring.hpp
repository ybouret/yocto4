#ifndef YOCTO_RX_MOD_CSTRING_INCLUDED
#define YOCTO_RX_MOD_CSTRING_INCLUDED 1


#include "yocto/rx/lexical/plugin.hpp"

namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            class mod_cstring : public plugin
            {
            public:
                explicit mod_cstring( const callback &cb);
                virtual ~mod_cstring() throw();
                string content;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(mod_cstring);
                virtual void enter();
                virtual void leave();
                virtual void *data() throw();
                void add( const token &tkn );
                
            };
        }
    }
}

#endif

