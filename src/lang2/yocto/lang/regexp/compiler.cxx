#include "yocto/lang/regexp/compiler.hxx"

namespace yocto
{
    namespace lang
    {

        RegExp:: ~RegExp() throw() {}

        RegExp:: RegExp(const char *expr, const size_t n, const p_dict *d) :
        curr(expr),
        last(expr+n),
        dict(d)
        {
            assert(expr!=NULL);
        }


    }
}