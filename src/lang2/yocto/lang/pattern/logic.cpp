#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        logical:: ~logical() throw()
        {
        }

        logical:: logical(uint32_t ID) throw() : pattern(ID), operands() {}

        logical:: logical(const logical &other) : pattern(other), operands(other.operands) {}

        void logical:: append( pattern *p ) throw()
        {
            assert(p);
            operands.push_back(p);
        }

    }
}
