#ifndef YOCTO_LANG_LEXICAL_PLUGIN_C_COMMENT_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_C_COMMENT_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            //! end of line comment
            class C_comment : public plugin
            {
            public:
                explicit C_comment(const string &id,
                                   lexer        &parent);
                virtual ~C_comment() throw();
                
                virtual pattern *trigger() const;
                virtual void     on_call(const token &);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(C_comment);
            };
            
        }
        
    }
    
}

#endif
