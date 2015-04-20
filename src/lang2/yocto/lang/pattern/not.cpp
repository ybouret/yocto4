#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        NOT:: ~NOT() throw() {}


        NOT:: NOT() throw() : logical( NOT::UUID ) {}

        NOT:: NOT(const NOT &other) : logical(other) {}

        pattern *NOT:: clone() const
        {
            return new NOT(*this);
        }

        bool NOT:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            
            return false;
        }

        logical *NOT::create()
        {
            return new NOT();
        }
        
    }
}
