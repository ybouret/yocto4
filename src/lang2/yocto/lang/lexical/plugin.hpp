#ifndef YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"

namespace yocto
{
    namespace lang
    {
        namespace  lexical
        {

            class plugin : public scanner
            {
            public:
                virtual ~plugin() throw();

                virtual  pattern *trigger() const = 0;
                virtual  void     on_call(const token &) = 0;

                void hook( scanner &scan )
                {
                    const callback cb(this, & plugin::on_call );
                    scan.call(name, trigger(), cb);
                }

            protected:
                //! set the scanner's name and link it to the lexer
                explicit plugin(const string &id, lexer &);

                //! ctor wrapper
                explicit plugin(const char   *id, lexer &);

                //lexer &get_lexer() { assert(lex); return *lex; }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
            };

        }
    }
}

#endif
