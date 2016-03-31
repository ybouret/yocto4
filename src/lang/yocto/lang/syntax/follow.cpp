#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/terminal.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            following_rule:: following_rule(const rule *r) throw() :
            next(0),
            prev(0),
            addr(r->derived),
            uuid(r->uuid)
            {
                assert(terminal::UUID==uuid || aggregate::UUID==uuid);
            }

            following_rule:: ~following_rule() throw()
            {
            }
        }

    }
}


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            following_rules:: following_rules() throw()
            {
            }

            following_rules:: ~following_rules() throw()
            {
            }
            
        }
    }

}