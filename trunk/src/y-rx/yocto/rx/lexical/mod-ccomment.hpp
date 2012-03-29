#ifndef YOCTO_RX_MOD_C_COMMENT_INCLUDED
#define YOCTO_RX_MOD_C_COMMENT_INCLUDED 1


#include "yocto/rx/lexical/plugin.hpp"

namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            class mod_ccomment : public plugin
            {
            public:
                explicit mod_ccomment();
                virtual ~mod_ccomment() throw();
                                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(mod_ccomment);
                virtual void enter();
                virtual void leave();
            };
            
        }
        
    }
}

#endif

