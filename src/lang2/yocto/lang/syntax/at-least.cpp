#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/ptr/auto.hpp"

#include <iostream>

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
            joker( make_label(), r),
            nmin(n)
            {
            }
            
            unsigned at_least:: counter = 0;
            
            string at_least:: make_label()
            {
                YOCTO_GIANT_LOCK();
                const string ans = vformat("AT_LEAST#%u",++counter);
                return ans;
            }
            
            bool at_least:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                std::cerr << "?>=%u" << nmin << std::endl;
                auto_ptr<xnode>  top( xnode::leaf(*this) );
                
                size_t count = 0;
                for(;;)
                {
                    xnode *sub = 0;
                    if(! jk.accept(sub,lxr,src,fp) )
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
                    std::cerr << "\t->=" << nmin << std::endl;
                    xnode::restore(top.yield(), lxr);
                    return false;
                }
                
                std::cerr << "\t+>=" << nmin << std::endl;
                grow(tree, top.yield());
                return true;
            }
        
            
            rule *zero_or_more( rule &r )
            {
                return new at_least(r,0);
            }

            rule *one_or_more( rule &r )
            {
                return new at_least(r,1);
            }
        }
        
        
        
    }
    
}
