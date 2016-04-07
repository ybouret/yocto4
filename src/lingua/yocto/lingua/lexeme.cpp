#include "yocto/lingua/lexeme.hpp"

namespace yocto
{
    namespace lingua
    {

        lexeme:: ~lexeme() throw() {}

        lexeme:: lexeme(const string &the_label,
                        const int     the_line,
                        token        &the_token) throw() :
        token(),
        next(0),
        prev(0),
        label(the_label),
        line(the_line)
        {
            swap_with(the_token);
        }

    }
}
