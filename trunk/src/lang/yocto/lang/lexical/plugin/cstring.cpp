#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/lang/lexer.hpp"

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
                //pattern *chars = posix::word();
                make("CHAR",chars,this, & cstring::gather );
                
                pattern *quote = compile("\\x22");
                back(quote, this, & cstring::leave );
                
                chars->graphviz("chars.dot");
                system("dot -Tpng -o chars.png chars.dot");
                quote->graphviz("quote.dot");
                system("dot -Tpng -o quote.png quote.dot");

            }
            
            
            pattern * cstring:: trigger() const
            {
                return single::create('"');
            }

            void cstring:: on_call( const token & )
            {
                std::cerr << "entering " << name << std::endl;
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
                std::cerr << "cstring=" << content << std::endl;
				mylex->unget(*this,content);
            }
            
        }
    }
}
