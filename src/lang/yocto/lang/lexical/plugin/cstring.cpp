#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/lexer.hpp"

#include "yocto/code/utils.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            cstring:: ~cstring() throw() {}
            
            
            cstring:: cstring( const string &id, lexer &parent ) :
            plugin(id,parent),
            content()
            {
                
                
                pattern *chars = compile("[\\x20-\\x21\\x23-\\x5b\\x5d-\\x7e]");
                make("CHAR",chars,this, & cstring::gather );
               
                static const char hex_esc_id[] = "string hexadecimal escape sequence";
                static const char hex_esc_rx[] = "\\\\x";
                call(hex_esc_id,compile(hex_esc_rx,NULL), this, & cstring::esc);
                
                static const char esc_id[] = "string escape sequence";
                static const char esc_rx[] = "\\\\";
                call(esc_id,compile(esc_rx,NULL), this, & cstring::esc );
                
                
               
                
                pattern *quote = compile("\\x22");
                back(quote, this, & cstring::leave );

                //______________________________________________________________
                //
                // on the fly creation: hexadecimal escape sequence
                //______________________________________________________________
                scanner &hex_esc = parent[hex_esc_id];
                hex_esc.back("[:xdigit:][:xdigit:]", this, & cstring::escape_hex);
                
                //______________________________________________________________
                //
                // on the fly creation: escape sequence
                //______________________________________________________________
                scanner &esc = parent[esc_id];
                esc.back( "[\\x22\\x27\\x3f\\x5c]", this, & cstring::escape1 );
                esc.back( "[0abfnrtv]",             this, & cstring::escape2 );
                
                
                
            }
            
            
            void cstring:: esc(const token &) throw()
            {
                // start any escape sequence => do nothing
            }
            
            void cstring:: escape1( const token &t )
            {
                assert(t.size==1);
                content.append(t.head->code);
            }
            
            void cstring:: escape2( const token &t )
            {
                assert(t.size==1);
                switch(t.head->code)
                {
                    case '0': content.append(char(0)); break;
                    case 'a': content.append('\a');break;
                    case 'b': content.append('\b');break;
                    case 'f': content.append('\f');break;
                    case 'n': content.append('\n');break;
                    case 'r': content.append('\r');break;
                    case 't': content.append('\t');break;
                    case 'v': content.append('\v');break;
                    default:
                        ;
                }
            }

            void cstring:: escape_hex(const token &t)
            {
                assert(t.size==2);
                const unsigned hi = hex2dec(t.head->code);
                const unsigned lo = hex2dec(t.tail->code);
                const unsigned C  = (hi<<4) | lo;
                content.append(char(C));
            }
            
            
            pattern * cstring:: trigger() const
            {
                return single::create('"');
            }

            void cstring:: on_call( const token & )
            {
                //std::cerr << "entering " << name << std::endl;
                content.clear();
            }
            
            bool cstring:: gather( const token &t )
            {
                //std::cerr << "content += " << t << std::endl;
                for(t_char *ch = t.head;ch;ch=ch->next)
                {
                    content.append(ch->code);
                }
                return false;
            }
            
            void cstring:: leave( const token & )
            {
                //std::cerr << "cstring=" << content << std::endl;
				mylex->unget(*this,content);
            }
            
        }
    }
}
