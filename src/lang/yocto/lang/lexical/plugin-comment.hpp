#ifndef YOCTO_LANG_LEXICAL_PLUGIN_COMMENT_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_COMMENT_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            //! end of line comment
            class comment : public plugin
            {
            public:
                explicit comment(const string &id,
                                 lexer        &parent,
                                 const string &expr);
                virtual ~comment() throw();
                
                virtual pattern *trigger() const;
                virtual void     on_call(const token &);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(comment);
                const string regexp;
            };
            
        }

    }

}

#endif
