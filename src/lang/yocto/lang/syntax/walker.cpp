#include "yocto/lang/syntax/walker.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            walker:: ~walker() throw()
            {
            }

            walker:: walker(const char *nsdb) throw() : hasher(nsdb)
            {
            }

            int walker:: hash(const string &label) const throw()
            {
                return hasher(label);
            }
            
        }
    }

}