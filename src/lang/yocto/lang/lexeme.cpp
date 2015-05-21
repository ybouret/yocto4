#include "yocto/lang/lexeme.hpp"

namespace yocto
{
    namespace lang
    {

        lexeme:: ~lexeme() throw() {}

        lexeme:: lexeme(const string &l, const int i) throw() :
        token(),
        next(0),
        prev(0),
        label(l),
        line(i)
        {

        }

        lexeme:: lexeme(const lexeme &other ) :
        token(other),
        next(0),
        prev(0),
        label(other.label),
        line(other.line)
        {
        }


    }

}
