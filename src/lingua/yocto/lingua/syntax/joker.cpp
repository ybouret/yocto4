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
                fp << "\",shape=circle,style=dotted";
                fp << "];\n";
                fp.viz(addr); fp << "->"; fp.viz(jk); fp << ";\n";
            }


            bool optional:: admit( YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS ) const
            {
                xnode *leaf = 0;
                if(jk->admit(leaf, lxr, src))
                {
                    if(leaf)
                    {
                        grow(tree,leaf);
                    }
                }
                return true;
            }

            bool optional:: admit_empty() const throw()
            {
                return true;
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
            at_least:: ~at_least() throw()
            {

            }


            at_least:: at_least(const string &id,
                                const rule   *r,
                                const size_t  n) :
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
                fp << "\",shape=invhouse,style=dotted";
                fp << "];\n";
                fp.viz(addr); fp << "->"; fp.viz(jk); fp << ";\n";
            }

            bool at_least :: admit( YOCTO_LINGUA_SYNTAX_RULE_ADMIT_ARGS ) const
            {
                //______________________________________________________________
                //
                // make a subtree
                //______________________________________________________________
                xnode          *leaves = xnode::create(*this);
                auto_ptr<xnode> guard(leaves);
                size_t          count = 0;

                //______________________________________________________________
                //
                // collect subtree
                //______________________________________________________________
                while(jk->admit(leaves, lxr, src))
                {
                    ++count;
                }

                //______________________________________________________________
                //
                // process
                //______________________________________________________________
                guard.forget();

                if(count>=nmin)
                {
                    grow(tree,leaves);
                    return true;
                }
                else
                {
                    xnode::back_to(lxr,leaves);
                    return false;
                }

            }

            bool at_least:: admit_empty() const throw()
            {
                if(nmin<=0)
                {
                    return true;
                }
                else
                {
                    return jk->admit_empty();
                }
            }


        }
    }
}
