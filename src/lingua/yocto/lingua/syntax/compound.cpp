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
        }

    }

}

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

        }
        
    }
    
}

