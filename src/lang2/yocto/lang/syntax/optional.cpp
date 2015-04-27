#include "yocto/lang/syntax/optional.hpp"


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            optional:: ~optional() throw() {}

            optional:: optional(const string &label, rule &r) :
            joker(label,r)
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

