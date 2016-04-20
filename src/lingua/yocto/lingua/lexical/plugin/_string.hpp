#ifndef YOCTO_LINGUA_LEXICAL_PLUGIN__STRING_INCLUDED
#define YOCTO_LINGUA_LEXICAL_PLUGIN__STRING_INCLUDED 1

#include "yocto/lingua/lexical/plugin.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            //! compile a C string
            class _string : public plugin
            {
            public:
                virtual ~_string() throw();
                explicit _string(const string &id,
                                 lexer        &lxr,
                                 const char   *delim);

                string content;
                virtual void on_call(const token&);

                //! retrieve C-style compiled string
                static string encode(const string &src);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(_string);
                void on_back(const token &);
                bool append(const token &);
                void on_esc(const token&);
                void on_esc_hex(const token&);

                
            };
            
        }
        
    }
    
}

#endif
