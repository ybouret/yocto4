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


            rule:: rule(const string &id) :
            next(0),
            prev(0),
            label(id)
            {
            }


            void rule:: grow( xnode * &tree, xnode *leaf ) const throw()
            {
                assert(leaf);
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


