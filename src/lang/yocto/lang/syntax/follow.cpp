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

#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/exception.hpp"

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


            void following_rules:: collect(const rule *r)
            {
                assert(r);
                switch(r->uuid)
                {
                    case terminal ::UUID:
                    case aggregate::UUID:
                        push_back( new following_rule(r) );
                        break;

                    case optional:: UUID:
                    case at_least:: UUID:
                        collect( static_cast<const rule*>(r->content()) );
                        break;

                    case alternate::UUID: {
                        const operands *ops = static_cast<const operands *>(r->content());
                        for(const logical::operand *op = ops->head; op; op=op->next)
                        {
                            collect(op->addr);
                        }
                    } break;

                    default:
                        throw exception("unexpected rule type in following_rules::collect");
                }
            }

        }
    }

}