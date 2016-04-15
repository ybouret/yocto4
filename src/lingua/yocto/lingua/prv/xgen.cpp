#include "yocto/lingua/prv/xgen.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }

            xgen:: xgen()
            {
            }


            lingua::parser *xgen:: generate(const xnode *tree, const bool output_files)
            {
                assert(tree);
                assert(aggregate::UUID==tree->origin->uuid);
                assert("XGEN"==tree->label());

                //______________________________________________________________
                //
                // using top level list
                //______________________________________________________________
                const xlist &top_level = *(tree->ch);    assert(top_level.size>0);
                const xnode *node      = top_level.head; assert("NAME"==node->label()); assert(node->terminal);
                

                return 0;
            }

        }

        
    }
}
