#include "yocto/lingua/lexer.hpp"

namespace yocto
{
    namespace lingua
    {

        lexer:: ~lexer() throw()
        {
        }

        lexer:: lexer() :
        line(0),
        dict(),
        root(0)
        {
        }
    }

}

