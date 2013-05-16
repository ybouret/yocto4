#include "yocto/lingua/lexer.hpp"

namespace yocto
{
    namespace lingua
    {
        lexer:: ~lexer() throw() {}
        
        lexer:: lexer( const string &id ) :
        name(id),
        line(0),
        scanners()
        {
        }
        
        
    }
}
