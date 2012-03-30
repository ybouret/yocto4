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
                //! constructor
                /** 
                 - back() on end of C comment
                 - make( endl, add )
                 - make( dot,  add )
                 */
                explicit mod_ccomment( const callback &cb );
                virtual ~mod_ccomment() throw();
                
                string content;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(mod_ccomment);
                virtual void enter();
                virtual void leave();
                virtual void *data() throw();
                
                void add( const token &tkn );
            };
            
        }
        
    }
}

#endif

