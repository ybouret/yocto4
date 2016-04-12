#include "yocto/lingua/syntax/compound.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            compound:: ~compound() throw()
            {
            }

            compound:: compound(const string &id, const uint32_t t) :
            rule(id,t),
            members()
            {
            }

            compound & compound:: operator<<( const rule &r )
            {
                members.append(&r);
                return *this;
            }

            void compound:: subviz(ios::ostream &fp) const
            {
                const rule *addr = this;
                for(const node_type *node = members.head; node; node=node->next)
                {
                    fp.viz(addr); fp << "->" ; fp.viz(node->addr); fp << ";\n";
                }
            }
        }

    }
}

#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            alternate:: ~alternate() throw() {}

            alternate:: alternate(const string &id) :
            compound(id,UUID)
            {
                YOCTO_LINGUA_SYNTAX_RULE_IS(alternate);
            }

            void alternate:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr); fp << "[label=\""; ios::graphviz_encode(label,fp); fp << "\",shape=egg];\n";
                subviz(fp);
            }

            bool alternate:: admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const
            {
                xnode *leaf = 0;
                for(const node_type *member = members.head; member; member=member->next)
                {
                    if(member->addr->admit(leaf, lxr, src) )
                    {
                        grow(tree,leaf);
                        return true;
                    }
                }
                return false;
            }
        }

    }

}

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            aggregate:: ~aggregate() throw() {}

            aggregate:: aggregate(const string &id) :
            compound(id,UUID)
            {
                YOCTO_LINGUA_SYNTAX_RULE_IS(aggregate);
            }


            void aggregate:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr); fp << "[label=\""; ios::graphviz_encode(label,fp); fp << "\",shape=house];\n";
                subviz(fp);
            }

            bool aggregate::admit(YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS) const
            {
                xnode *leaves = xnode::create(*this);
                auto_ptr<xnode> guard(leaves);
                for(const node_type *member = members.head; member; member=member->next)
                {
                    if(!member->addr->admit(leaves,lxr,src))
                    {
                        guard.forget();
                        xnode::back_to(lxr,leaves);
                        return false;
                    }
                }

                guard.forget();
                grow(tree,leaves);
                return true;
            }

        }
        
    }
    
}

