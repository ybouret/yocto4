#include "yocto/lang/syntax/optional.hpp"


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            unsigned optional::counter;
            
            optional:: ~optional() throw() {}


            string optional:: make_label()
            {
                YOCTO_GIANT_LOCK();
                const string ans = vformat("OPT#%u", ++counter);
                return ans;
            }

            optional:: optional(rule &r) :
            joker(make_label(),r)
            {
            }

            bool optional:: accept(Y_LANG_SYNTAX_RULE_ACCEPT_ARGS) const
            {
                xnode *sub = 0;
                if(jk.accept(sub,lxr,src,fp))
                {
                    if(sub)
                    {
                        grow(tree,sub);
                    }
                }
                return true;
            }
            
            
        }

    }

}

