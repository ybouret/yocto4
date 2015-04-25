#ifndef YOCTO_LANG_LEX_PLUGIN_CSTRING_INCLUDED
#define YOCTO_LANG_LEX_PLUGIN_CSTRING_INCLUDED 1


#include "yocto/lang/lexical/plugin.hpp"
#include "yocto/ptr/auto.hpp"

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
                explicit cstring(const char   *id, lexer &parent);
                virtual pattern *trigger() const;
                virtual void     on_call(const token &);


            private:
                string            content;
                void on_quit(const token &);
                bool on_char(const token &tkn);
                bool on_esc_char( const token &tkn );
                
            };
            
        }
        
    }
}

#endif
