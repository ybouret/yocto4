#include "yocto/lang/lexical/plugin/ccomment.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {

            ccomment:: ~ccomment() throw()
            {

            }

            ccomment:: ccomment(const char *id,
                                lexer      &LX,
                                const char *ini, const char *end) :
            plugin(id,LX),
            motif( regexp(ini,dict) )
            {
                back(end);
                drop("CHAR", ".");
                endl("ENDL");
            }

            void ccomment:: on_call(const token &)
            {
            }

            pattern * ccomment:: trigger() const
            {
                return motif->clone();
            }
        }
        
    }
    
}
