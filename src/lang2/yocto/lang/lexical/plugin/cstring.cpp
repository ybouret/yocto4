#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/pattern/basic.hpp"

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

            }

            void cstring:: on_call(const token &)
            {
                content.clear();
            }

            pattern * cstring:: trigger() const { return single::create('"'); }
            
        }
        
    }
    
}
