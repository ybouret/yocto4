#include "yocto/lang/lexical/plugin/comment.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {

            comment:: ~comment() throw()
            {
            }

            comment:: comment( const char *id, lexer &LX, const char *marker) :
            plugin(id,LX),
            motif( regexp(marker,dict) )
            {
                drop("CHAR", ".");
                back_newline();
            }

            void comment:: on_call(const token &)
            {
            }

            pattern * comment:: trigger() const
            {
                return motif->clone();
            }
        }
        
    }
    
}
