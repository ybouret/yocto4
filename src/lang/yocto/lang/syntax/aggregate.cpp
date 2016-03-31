#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            aggregate:: ~aggregate() throw()
            {
            }

            aggregate:: aggregate(const string &id, const property ppty) :
            logical(id, UUID),
            modifier(ppty)
            {
                Y_LANG_SYNTAX_DERIVED(aggregate);
                
                switch(modifier)
                {
                    case jettison:
                    case univocal:
                        throw exception("invalid modifier for '%s", id.c_str());
                    default:;
                }
            }


            bool aggregate:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {

                if(size<=0)
                    throw exception("aggregate '%s' has no operand", label.c_str());

                auto_ptr<xnode> agg( xnode::leaf(*this,modifier) );

                for(const operand *node=head;node;node=node->next)
                {
                    const rule &r   = *(node->addr);
                    xnode      *sub = 0;
                    if(!r.accept(sub, lxr, src, fp))
                    {
                        assert(0==sub);
                        xnode::restore(agg.yield(), lxr);
                        return false;
                    }
                    
                    if(sub)
                    {
                        agg->append(sub);
                    }
                }
                
                grow(tree,agg.yield());
                return true;
            }

            void aggregate:: viz( ios::ostream &fp ) const
            {
                const char *style = "solid";
                switch(modifier)
                {
                    case mergeOne: style = "dotted"; break;


                    case mergeAll: style = "filled"; break;
                        
                    default:
                        break;
                }
                fp.viz((const rule*)this);
                fp("[shape=house,style=%s,label=\"",style);
                encode_label(fp);
                fp("\"];\n");
            }

            void aggregate:: lnk( ios::ostream &fp ) const
            {
                for(const operand *node=head;node;node=node->next)
                {
                    fp.viz( (const rule*)this ); fp << " -> "; fp.viz(node->addr); fp << ";\n";
                }
            }

            aggregate & aggregate:: operator+=( rule &r )
            {
                add(r);
                return *this;
            }

           

        }
    }
}

