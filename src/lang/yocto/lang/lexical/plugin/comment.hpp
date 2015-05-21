#ifndef YOCTO_LANG_LEX_PLUGIN_COMMENT_INCLUDED
#define YOCTO_LANG_LEX_PLUGIN_COMMENT_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/ptr/auto.hpp"

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
                virtual ~comment() throw();
                explicit comment(const char *id, lexer &, const char *marker);

                virtual pattern *trigger() const;
                virtual void     on_call(const token &);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(comment);
                auto_ptr<pattern> motif;
            };


        }
    }
}

#endif
