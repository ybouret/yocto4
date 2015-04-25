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
                
                const string EscID = name + "Esc";
                scanner &Esc = parent.declare(EscID);
                
                //______________________________________________________________
                //
                // default char
                //______________________________________________________________
                make("CHAR","[\\x20-\\x21\\x23-\\x5b\\x5d-\\x7f]",this, & cstring::on_char);
                
                //______________________________________________________________
                //
                // call an escape sequence
                //______________________________________________________________
                const callback cb(this, &scanner::discard_cb);
                call(EscID, single::create('\\'),cb);
                
                //______________________________________________________________
                //
                // End of string
                //______________________________________________________________
                {
                    const callback cb(this, & cstring::on_quit);
                    back( single::create('"'), cb);
                }
                
                {
                    const string esc_char = "esc_char";
                    const action esc_char_add(this, & cstring::on_esc_char);
                    Esc.make(esc_char, any1::create(), esc_char_add);
                }
            }
            
            bool cstring:: on_char(const token &tkn )
            {
                assert(1==tkn.size);
                content.append( char(tkn.head->code) );
                return false;
            }
            
            bool cstring:: on_esc_char(const token &tkn)
            {
                assert(1==tkn.size);
                content.append( char(tkn.head->code) );
                return false;
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
