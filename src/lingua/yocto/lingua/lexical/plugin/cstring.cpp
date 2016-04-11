#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/lexer.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/utf8.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            cstring:: ~cstring() throw()
            {
            }


            cstring:: cstring(const string &id, lexer &lxr) :
            plugin(id,lxr,"\""),
            content()
            {
                back(trigger, this, &cstring::on_back);

                const string esc_label = id + "_esc";
                call(esc_label, "\\\\", this, &scanner::drop_cb);

                //______________________________________________________________
                //
                // escape sequence
                //______________________________________________________________
                {
                    scanner &esc = lxr.declare(esc_label);
                    esc.back("x[:xdigit:][:xdigit:]",this, &cstring::on_esc_hex);
                    esc.back("[\\x00-\\xff]", this, &cstring::on_esc);
                }

                //______________________________________________________________
                //
                // other char
                //______________________________________________________________
                {
                    const string label = id + "_char";
                    make(label,UTF8::ValidCharRX,this,&cstring::append);
                }


            }


            void cstring:: on_call(const token &)
            {
                content.clear();
            }

            void cstring:: on_back(const token &)
            {
                lex->unget(*this,line,content);
            }

            bool cstring:: append(const token &t)
            {
                for(const t_char *ch = t.head;ch;ch=ch->next)
                {
                    content.append(char(ch->code));
                }
                return false;
            }

            void cstring:: on_esc(const token &t)
            {
                assert(1==t.size);
                const char C(t.head->code);
                switch(C)
                {
                    case 'n': content.append('\n'); break;
                    case 'r': content.append('\r'); break;
                    case 't': content.append('\t'); break;
                    case 'f': content.append('\f'); break;
                    case 'b': content.append('\b'); break;


                    case '\\':
                    case '\"':
                    case '/':
                        content.append(C);
                        break;


                    default:
                        throw exception("%d:{%s}<%s>: invalid escape sequence '\\%c...'", line, lex->name.c_str(), name.c_str(),C);
                }
            }

            void cstring:: on_esc_hex(const token &t)
            {
                assert(3==t.size);
                const t_char *ch = t.tail;
                const uint8_t h = 16 * hex2dec(char(ch->prev->code)) + hex2dec( char(ch->code) );
                content.append( char(h) );
            }

        }
    }
}
