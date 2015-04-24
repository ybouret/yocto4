#ifndef YOCTO_LANG_LEX_PLUGIN_CCOMMENT_INCLUDED
#define YOCTO_LANG_LEX_PLUGIN_CCOMMENT_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {

            //! chunk comments (aka C-Comment)
            class ccomment : public plugin
            {
            public:
                virtual ~ccomment() throw();
                explicit ccomment(const char *id, lexer &, const char *ini, const char *end);

                virtual pattern *trigger() const;
                virtual void     onEnter(const token &);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(ccomment);
                auto_ptr<pattern>  motif;
            };
            
            
        }
    }
}

#endif
