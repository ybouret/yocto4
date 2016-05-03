#ifndef YOCTO_LANG_LEX_PLUGIN_RSTRING_INCLUDED
#define YOCTO_LANG_LEX_PLUGIN_RSTRING_INCLUDED 1


#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{

    namespace lang
    {
        namespace lexical
        {

            //! start raw string, verbatim between single quotes...
            class rstring : public plugin
            {
            public:
                virtual ~rstring() throw();
                explicit rstring(const char   *id, lexer &parent);
                virtual pattern *trigger() const;
                virtual void     on_call(const token &);

            private:
                void on_quit(const token &);
                bool gather(const token &);
                string content;
                YOCTO_DISABLE_COPY_AND_ASSIGN(rstring);
            };


        }
    }
}

#endif

