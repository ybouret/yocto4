#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

#include "yocto/code/utils.hpp"

#include <iostream>

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
                
                const string EscID    = name + "Esc";
                scanner     &Esc      = parent.declare(EscID);
                const string EscHexID = name + "EscHex";
                scanner     &EscHex   = parent.declare(EscHexID);
                const string EscUni4ID = name + "EscUni4";
                scanner     &EscUni4   = parent.declare(EscUni4ID);

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
                call(EscID, single::create(0x5c),cb);
                
                //______________________________________________________________
                //
                // End of string
                //______________________________________________________________
                {
                    const callback cb(this, & cstring::on_quit);
                    back( single::create('"'), cb);
                }
                make("BADCHAR",".",this, & cstring::on_bad_char);

                //______________________________________________________________
                //
                // Esc sequence sub scanner
                //______________________________________________________________
                {

                    // Hexadecimal ?
                    const callback esc_jmp_cb(this, & scanner::discard_cb);
                    Esc.jump(EscHexID, single::create('x'), esc_jmp_cb);

                    // Unicode
                    Esc.jump(EscUni4ID, single::create('u'), esc_jmp_cb);


                    // default: single escape char
                    const callback esc_char_cb(this, & cstring::on_esc_char);
                    Esc.back(any1::create(), esc_char_cb);
                }

                //______________________________________________________________
                //
                // EscHex sequence sub scanner
                //______________________________________________________________
                {
                    EscHex.back("[:xdigit:][:xdigit:]",this,&cstring::on_esc_hexa);

                    const callback bad_hexa(this, & cstring::on_bad_hexa);

                    EscHex.back(any1::create(),bad_hexa);
                }


                //______________________________________________________________
                //
                // EscUni sequence sub scanner
                //______________________________________________________________
                {
                    EscUni4.back("[:xdigit:][:xdigit:][:xdigit:][:xdigit:]",this,&cstring::on_esc_uni4);

                    const callback bad_uni4(this, & cstring::on_bad_uni4);

                    EscUni4.back(any1::create(),bad_uni4);
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

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            bool cstring:: on_char(const token &tkn )
            {
                assert(1==tkn.size);
                content.append( char(tkn.head->code) );
                return false;
            }

            bool cstring:: on_bad_char(const token &tkn )
            {
                assert(1==tkn.size);
                throw exception("%s: invalid char 0x%08x", name.c_str(), unsigned(tkn.head->code));
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
                    case 'a': C = '\a'; break;
                    case 'b': C = '\b'; break;
                    case 'f': C = '\f'; break;
                    case '0': C =  0;   break;

                    case '\\':
                    case '"':
                    case '\'':
                        break;

                    default:
                        throw exception("%s: unknown escape sequence %0x02u", name.c_str(), C);
                }

                content.append( C );
            }

            void cstring:: on_esc_hexa(const token &tkn )
            {
                assert(2==tkn.size);
                const int C = hex2dec(tkn.head->code) * 16 + hex2dec(tkn.head->next->code);
                content.append(C);
            }

            void cstring:: on_bad_hexa(const token &)
            {
                throw exception("%s: invalid hexadecimal escape sequence", name.c_str());
            }
        }

    }

}

#include "yocto/string/utf8.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            void cstring:: on_esc_uni4(const token &tkn)
            {
                assert(4==tkn.size);
                uint32_t u = 0;
                for(const t_char *ch = tkn.head; ch; ch=ch->next)
                {
                    u <<= 4;
                    u |= uint32_t(hex2dec(ch->code));
                }
                if(!UTF8::Encode(u,content))
                {
                    throw exception("%s: invalid UTF8 code point 0x%08x", name.c_str(), u);
                }
            }

            void cstring:: on_bad_uni4(const token &)
            {
                throw exception("%s: invalid unicode escape sequence", name.c_str());
            }

        }
    }

}

