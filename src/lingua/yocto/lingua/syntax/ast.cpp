#include "yocto/lingua/syntax/rule.hpp"
#include "yocto/lingua/syntax/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


            xnode * xnode:: ast(xnode *node) throw()
            {
                //______________________________________________________________
                //
                // stop condition
                //______________________________________________________________
                if(!node)
                {
                    return NULL;
                }

                //______________________________________________________________
                //
                // process terminal
                //______________________________________________________________
                if(node->terminal)
                {
                    switch(node->origin->flags)
                    {
                        case property::univocal: node->lx->clear(); return node;
                        case property::jettison: delete node;       return NULL;
                        default:
                            break;
                    }
                    return node;
                }

                //______________________________________________________________
                //
                // process internal
                //______________________________________________________________
                xlist &ch = *(node->ch);
                xlist  stk;
                while(ch.size)
                {
                    xnode *sub = ast(ch.pop_front()); //!< recursive call
                    if(!sub) continue;

                    stk.push_back(sub);

                }
                ch.swap_with(stk);
                return node;
            }

        }

    }
}
