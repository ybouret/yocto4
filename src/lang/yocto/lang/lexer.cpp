#include "yocto/lang/lexer.hpp"

namespace yocto
{
    namespace lang
    {
        
        lexer:: ~lexer() throw()
        {
        }
        
#define Y_LEXER_CTOR() name(id), line(1)
        
        lexer:: lexer(const string &id) :
        Y_LEXER_CTOR()
        {
        }
        
        lexer:: lexer( const char *id ) :
        Y_LEXER_CTOR()
        {
        }
        
        
    }
}
