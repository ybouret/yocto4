#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/lexer.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/string/utf8.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace lang
    {
        namespace lexical
        {
            
            cstring:: ~cstring() throw() {}
            
            
            cstring:: cstring( const string &id, lexer &parent ) :
            plugin(id,parent),
            content(),
            count(0)
            {
                setup(parent);
            }
            
            
            cstring:: cstring(const char   *id, lexer &parent) :
            plugin(id,parent),
            content(),
            count(0)
            {
                setup(parent);
            }
            
            void cstring:: esc(const token &) throw()
            {
                // start any escape sequence
                count = 0;
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
            
            
            void cstring:: setup(lexer &parent)
            {
                pattern *chars = compile("[\\x20-\\x21\\x23-\\x5b\\x5d-\\x7e]");
                make("CHAR",chars,this, & cstring::gather );
                
                static const char hex_esc_id[] = "string hexadecimal escape sequence";
                static const char hex_esc_rx[] = "\\\\x";
                call(hex_esc_id,compile(hex_esc_rx,NULL), this, & cstring::esc);
                
                static const char esc_id[] = "string escape sequence";
                static const char esc_rx[] = "\\\\";
                call(esc_id,compile(esc_rx,NULL), this, & cstring::esc );
                
                static const char u2_id[] = "unicode+nnnn sequence";
                static const char u2_rx[] = "\\\\u";
                call(u2_id,compile(u2_rx,NULL), this, &cstring::esc);
                
                
                static const char u4_id[] = "unicode+nnnnnnnn sequence";
                static const char u4_rx[] = "\\\\U";
                call(u4_id,compile(u4_rx,NULL), this, &cstring::esc);
                
                //______________________________________________________________
                //
                // default: back on quote
                //______________________________________________________________
                pattern *quote = compile("\\x22");
                back(quote, this, & cstring::leave );
                
                //______________________________________________________________
                //
                // on the fly creation: hexadecimal escape sequence
                //______________________________________________________________
                {
                    scanner &hex_esc = parent[hex_esc_id];
                    hex_esc.back("[:xdigit:][:xdigit:]", this, & cstring::escape_hex);
                }
                
                //______________________________________________________________
                //
                // on the fly creation: escape sequence
                //______________________________________________________________
                {
                    scanner &esc = parent[esc_id];
                    esc.back( "[\\x22\\x27\\x3f\\x5c]", this, & cstring::escape1 );
                    esc.back( "[0abfnrtv]",             this, & cstring::escape2 );
                }
                
                //______________________________________________________________
                //
                // on the fly creation: u+nnnn sequence
                //______________________________________________________________
                {
                    scanner &u2 = parent[u2_id];
                    u2.back( "[:xdigit:][:xdigit:][:xdigit:][:xdigit:]", this, & cstring::escape_u);
                }
                
                //______________________________________________________________
                //
                // on the fly creation: u+nnnn sequence
                //______________________________________________________________
                {
                    scanner &u4 = parent[u4_id];
                    u4.back( "[:xdigit:][:xdigit:][:xdigit:][:xdigit:][:xdigit:][:xdigit:][:xdigit:][:xdigit:]", this, & cstring::escape_U);
                }

            }
            
            void cstring:: escape_u(const token &t)
            {
                assert(4==t.size);
                uint32_t w = 0;
                for(const t_char *ch = t.head;ch;ch=ch->next)
                {
                    w <<= 4;
                    w  |= hex2dec(ch->code);
                }
                if(!UTF8::Encode(w,content))
                    throw exception("%d: invalid UTF8 u%04x", line, w);
            }
            
            void cstring::escape_U(const token &t)
            {
                assert(8==t.size);
                uint32_t w = 0;
                for(const t_char *ch = t.head;ch;ch=ch->next)
                {
                    w <<= 4;
                    w  |= hex2dec(ch->code);
                }
                if(!UTF8::Encode(w,content))
                    throw exception("%d: invalid UTF8 u%08x", line, w);
            }
            
        }
    }
}
