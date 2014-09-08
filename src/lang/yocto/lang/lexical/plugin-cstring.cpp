#include "yocto/lang/lexical/plugin-cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/posix.hpp"

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
                make("CHAR","[:word:]",this, & cstring::gather );
                back(single::create('"'), this, & cstring::leave );
            }
            
            
            pattern * cstring:: trigger() const
            {
                return single::create('"');
            }

            void cstring:: enter( const token & )
            {
                std::cerr << "entering " << name << std::endl;
                content.clear();
            }
            
            bool cstring:: gather( const token &t )
            {
                std::cerr << "content += " << t << std::endl;
                for(t_char *ch = t.head;ch;ch=ch->next)
                {
                    content.append(ch->code);
                }
                return true;
            }
            
            void cstring:: leave( const token & )
            {
                std::cerr << "cstring=" << content << std::endl;
                exit(1);
            }
            
        }
    }
}
