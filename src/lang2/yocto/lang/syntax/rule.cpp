#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            rule:: ~rule() throw()
            {
            }
            
            
            rule:: rule(const string &id, uint32_t uu) :
            next(0),
            prev(0),
            label(id),
            uuid(uu)
            {
            }
            
            
            void rule:: grow( xnode * &tree, xnode *leaf ) const throw()
            {
                if(leaf)
                {
                    if(!tree)
                    {
                        tree = leaf;
                    }
                    else
                    {
                        assert(!(tree->terminal));
                        tree->append(leaf);
                    }
                }
            }
            
        }
    }
}


