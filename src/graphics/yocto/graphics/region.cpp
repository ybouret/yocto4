#include "yocto/graphics/region.hpp"
#include "yocto/code/bzset.hpp"

#include <cstring>

namespace yocto
{

    namespace graphics
    {
    }
    
}

namespace yocto
{

    namespace graphics
    {
        region:: region() throw() : regxels()
        {
        }

        region:: ~region() throw()
        {
        }

        region:: region(const region &other) : regxels()
        {
            for(const regxel *rgx = other.head; rgx; rgx=rgx->next)
            {
                regxel *node = new regxel();
                node->r = rgx->r;
                push_back(node);
            }
        }

        void region::load(const pixmapf &pxm) throw()
        {
            for(regxel *node=head;node;node=node->next)
            {
                node->value = pxm[node->r];
            }
        }


    }
}
