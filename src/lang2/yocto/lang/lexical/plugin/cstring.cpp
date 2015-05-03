#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            cstring:: ~cstring() throw()
            {
                
            }
            
            cstring:: cstring(const char *id,
                              lexer      &parent) :
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
                
                //______________________________________________________________
                //
                // Esc sequence sub scanner
                //______________________________________________________________
                {
                    const string esc_char = "esc_char";
                    const callback esc_char_add(this, & cstring::on_esc_char);
                    
                    // default
                    Esc.back(any1::create(), esc_char_add);
                }
                
            }
            
            bool cstring:: on_char(const token &tkn )
            {
                assert(1==tkn.size);
                content.append( char(tkn.head->code) );
                return false;
            }
            
            void cstring:: on_esc_char(const token &tkn)
            {
                assert(1==tkn.size);
                
                // single char, not hexa
                char C = tkn.head->code;
                switch(C)
                {
                    case 't': C = '\t'; break;
                    case 'n': C = '\n'; break;
                    case 'r': C = '\r'; break;
                    case '0': C = 0;    break;
                        
                    case '\\':
                    case '"':
                    case '\'':
                        break;
                        
                    default:
                        throw exception("%s: unknown escape sequence %0x02u", name.c_str(), C);
                }
                
                content.append( C );
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
