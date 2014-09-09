#ifndef YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"

namespace yocto
{
 
    namespace lang
    {
        namespace lexical
        {
            
            class plugin : public scanner
            {
            public:
                typedef intr_ptr<string,plugin> pointer;
                
                virtual ~plugin() throw();
                
                virtual pattern *trigger() const = 0;
                virtual void     on_call(const token &) = 0;
                
            protected:
                explicit plugin(const string &id,lexer &parent);
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
            };
            
        }
    }
}


#endif

