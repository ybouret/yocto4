#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/lexer.hpp"

namespace yocto
{

    namespace lang
    {
        namespace lexical
        {

            cstring:: ~cstring() throw()
            {

            }

            cstring:: cstring(const char *id, lexer &parent) :
            plugin(id,parent),
            content()
            {

                
                //______________________________________________________________
                //
                // default char
                //______________________________________________________________
                emit("CHAR","[\\x20-\\x21\\x23-\\x5b\\x5d-\\x7f]");
                
                
                //______________________________________________________________
                //
                // End of string
                //______________________________________________________________
                {
                    const callback cb(this, & cstring::on_quit);
                    back( single::create('"'), cb);
                }
            }

            void cstring:: on_call(const token &)
            {
                content.clear();
            }

            void cstring:: on_quit(const token &)
            {
                lex->unget(*this, content);
            }

            pattern * cstring:: trigger() const { return single::create('"'); }
            
        }
        
    }
    
}
