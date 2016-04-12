#include "yocto/lingua/syntax/term.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal(const string &id) :
            rule(id,UUID)
            {
                YOCTO_LINGUA_PATTERN_IS(terminal);
            }

        }
    }
}