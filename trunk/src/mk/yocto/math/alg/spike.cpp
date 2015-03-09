#include "yocto/math/alg/spike.hpp"

namespace yocto
{
    namespace math
    {
        spike::index_node::  index_node(size_t i) throw() : value(i), next(0), prev(0) {}
        spike::index_node:: ~index_node() throw() {}

        int spike::index_node::compare(const index_node *lhs, const index_node *rhs) throw()
        {
            const size_t L = lhs->value;
            const size_t R = rhs->value;
            return (L<R) ? -1 : ( (R<L) ? 1  : 0 );
        }
    }

}

namespace yocto
{
    namespace math
    {
        spike:: spike( size_t pos ) throw() :
        position(pos),
        indices()
        {

        }

        spike:: ~spike() throw()
        {
        }


    }
}
