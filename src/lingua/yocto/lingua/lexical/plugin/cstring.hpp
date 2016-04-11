#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_CSTRING_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_CSTRING_INCLUDED 1

#include "yocto/lingua/lexical/plugin.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            class cstring : public plugin
            {
            public:
                virtual ~cstring() throw();
                explicit cstring(const string &id,
                                 lexer        &lxr);

                string content;
                virtual void on_call(const token&);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(cstring);
                void on_back(const token &);
                bool append(const token &);
                void on_esc(const token&);
                void on_esc_hex(const token&);


            };

        }

    }

}

#endif
