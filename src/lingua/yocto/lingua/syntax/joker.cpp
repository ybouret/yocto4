#include "yocto/lingua/syntax/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            joker:: ~joker() throw()
            {
            }

            joker:: joker(const string  &id,
                          const uint32_t t,
                          const rule    *r) :
            rule(id,t),
            jk(r)
            {
                assert(jk!=0);
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

            optional:: ~optional() throw()
            {
            }

            optional:: optional(const string &id, const rule *r) :
            joker(id,UUID,r)
            {
                YOCTO_LINGUA_SYNTAX_RULE_IS(optional);
            }

            void optional:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr);
                fp << "[label=\""; ios::graphviz_encode(label,fp);
                fp << "\",shape=circle";
                fp << "];\n";
                fp.viz(addr); fp << "->"; fp.viz(jk); fp << ";\n";
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
            at_least:: ~at_least() throw()
            {

            }


            at_least:: at_least(const string &id, const rule *r, const size_t n) :
            joker(id,UUID,r),
            nmin(n)
            {
                YOCTO_LINGUA_SYNTAX_RULE_IS(at_least);
            }

            void at_least:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr);
                fp << "[label=\""; ios::graphviz_encode(label,fp);
                fp << "\",shape=trapezium";
                fp << "];\n";
                fp.viz(addr); fp << "->"; fp.viz(jk); fp << ";\n";
            }
        }
    }
}
