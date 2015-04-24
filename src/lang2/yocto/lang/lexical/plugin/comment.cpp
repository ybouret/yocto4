#include "yocto/lang/lexical/plugin/comment.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {

            comment:: ~comment() throw()
            {
                assert(motif);
                delete motif;
                motif=0;
            }

            comment:: comment( lexer &LX, const char *marker) :
            plugin("commment",LX),
            motif( regexp(marker,dict) )
            {
                try
                {
                    drop("CHAR", ".");
                    back_newline();
                }
                catch(...)
                {
                    delete motif;
                    throw;
                }
            }

            void comment:: onEnter(const token &)
            {
            }

            pattern * comment:: trigger() const
            {
                assert(motif);
                return motif->clone();
            }
        }
        
    }
    
}
