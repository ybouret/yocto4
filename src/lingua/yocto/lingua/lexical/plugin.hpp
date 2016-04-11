#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN_INCLUDED 1

#include "yocto/lingua/lexical/scanner.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            class plugin : public scanner
            {
            public:
                virtual ~plugin() throw();

                virtual void on_call(const token &) = 0;

                void hook( scanner &target );



            protected:
                explicit plugin(const string &id,
                                lexer        &lxr,
                                const string &expr);

                explicit plugin(const string &id,
                                lexer        &lxr,
                                const char   *expr);
                
                const string trigger;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
            };
        }
    }
}
#endif
