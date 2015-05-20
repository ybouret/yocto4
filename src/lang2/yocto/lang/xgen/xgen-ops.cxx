#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include <iostream>

#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            void xgen:: mark_visited(rule *r,const char *ctx)
            {
                assert(r);
                assert(ctx);
                assert(!visited.search(r));
                if(!visited.insert(r))
                {
                    throw exception("%s: unexpected visited failure while %s",name,ctx);
                }
            }

            void xgen:: validate( rule *r )
            {
                assert(r);
                if(! visited.search(r) )
                {
                    throw exception("%s: rule '%s' is disconnected from '%s'", name, r->label.c_str(), xprs->top_level().label.c_str());
                }
            }

            void xgen:: check_connectivity(rule *r)
            {
                assert(r);
                if(visited.search(r)) return;
                mark_visited(r, "checking connectivity");

                switch(r->uuid)
                {
                    case aggregate::UUID:
                    case alternate::UUID: {
                        assert(r->content());
                        operands *ops = static_cast<operands *>(r->content());
                        for(logical::operand *op=ops->head;op;op=op->next)
                        {
                            check_connectivity( op->addr );
                        }
                    } break;

                    case optional::UUID:
                    case at_least::UUID: {
                        assert(r->content());
                        check_connectivity( static_cast<rule *>(r->content()) );
                    } break;

                    default:
                        break;
                }
            }


            void xgen:: check_semantic(rule *r) throw()
            {
                assert(r);
                if(visited.search(r)) return;
                mark_visited(r, "checking semantic");

                // propagate
                switch(r->uuid)
                {
                    case aggregate::UUID:
                    case alternate::UUID: {
                        assert(r->content());
                        operands *ops = static_cast<operands *>(r->content());
                        for(logical::operand *op=ops->head;op;op=op->next)
                        {
                            check_semantic( op->addr );
                        }
                    } break;

                    case optional::UUID:
                    case at_least::UUID: {
                        assert(r->content());
                        check_semantic( static_cast<rule *>(r->content()) );
                    } break;

                    default:
                        break;
                }

                // would jettison RAW in big enough aggregates
                if(r->uuid==aggregate::UUID)
                {
                    std::cerr << "\t\tsemantic check in " << r->label << std::endl;
                    assert(r->content());
                    operands *ops = static_cast<operands *>(r->content());
                    if(ops->size>1)
                    {
                        for(logical::operand *op=ops->head;op;op=op->next)
                        {
                            rule *sub  = op->addr;
                            if(terminal::UUID==sub->uuid)
                            {
                                if(raw.search(sub->label))
                                {
                                    *static_cast<property *>(sub->content()) = jettison;
                                }
                            }
                        }
                    }
                }


            }
            


        }

    }
}
