#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/ptr/auto.hpp"


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {


            at_least:: ~at_least() throw()
            {
            }

            at_least:: at_least(rule &r, size_t n) :
            joker( make_label(n), r, UUID),
            nmin(n)
            {
            }

            unsigned at_least:: counter = 0;

            string at_least:: make_label(size_t n)
            {
                YOCTO_GIANT_LOCK();
                const string ans = vformat(">=%u#%u",unsigned(n),++counter);
                return ans;
            }

            bool at_least:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                auto_ptr<xnode>  top( xnode::leaf(*this,mergeAll) );

                size_t count = 0;
                for(;;)
                {
                    xnode *sub = 0;
                    if(! jk->accept(sub,lxr,src,fp) )
                    {
                        assert(0==sub);
                        break;
                    }
                    ++count;
                    if(sub)
                    {
                        top->append(sub);
                    }
                }

                if(count<nmin)
                {
                    xnode::restore(top.yield(), lxr);
                    return false;
                }

                grow(tree, top.yield());
                return true;
            }


            void at_least:: viz( ios::ostream &fp ) const
            {
                fp.viz((const rule *)this);
                fp("[shape=doublecircle,label=\">=%u\"];\n",unsigned(nmin));
            }

            void at_least:: lnk( ios::ostream &fp ) const
            {
                fp.viz( (const rule*)this ); fp << " -> "; fp.viz(jk); fp << ";\n";
            }
        }

        
        
    }
    
}
