#ifndef YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED
#define YOCTO_LANG_LEXICAL_PLUGIN_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"

namespace yocto
{
    namespace lang
    {
        namespace  lexical
        {

            //! base class for plugin
            /**
             a plugin need to be loaded by
             a lexer (and then it becomes one of
             its scanner).
             Then the plugin must be hooked to a former scanner.
             */
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


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
            };

        }
    }
}

#endif
