#ifndef YOCTO_LANG_LEXICAL_PLUGIN_CSTRING_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_CSTRING_INCLUDED 1

#include "yocto/lang/lexical/plugin.hpp"

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
                explicit cstring(const string &id, lexer &parent);
                explicit cstring(const char   *id, lexer &parent);
                virtual pattern *trigger() const;
                virtual void     on_call(const token &);
                
                
            private:
                string content;
                YOCTO_DISABLE_COPY_AND_ASSIGN(cstring);
                void leave(const token & );
                bool gather(const token &);
                void esc(const token &) throw();
                void escape1(const token &);    //!< direct escape sequence
                void escape2(const token &);    //!< coded  escape sequence
                void escape_hex(const token &); //!< hexadecimal
                void escape_u(const token &);   //!< unicode 16 bits
                void escape_U(const token &);   //!< unicode 32 bits
                void setup(lexer &parent);
                
                bool invalid_esc(const token &);
                bool invalid_hex(const token &);
                bool invalid_u(const token &);
                bool invalid_U(const token &);
                
            };
            
        }
        
    }
    
}

#endif
