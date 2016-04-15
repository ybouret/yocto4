#include "yocto/lingua/syntax/rule.hpp"
#include "yocto/lingua/syntax/joker.hpp"
#include "yocto/lingua/syntax/compound.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            static inline
            bool __collapse(const xnode *sub) throw()
            {
                assert(sub);
                switch(sub->origin->uuid)
                {
                    case optional::UUID:
                    case at_least::UUID:
                        return true;

                    case aggregate::UUID:
                        switch(sub->origin->flags)
                    {
                        case property::jettison: return true;
                        case property::noSingle: return sub->ch->size <= 1;
                        default:                 return false;
                    }

                    default:
                        break;
                }
                return false;
            }

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
                    if(!sub) continue;                //!< was deleted

                    //__________________________________________________________
                    //
                    // collapsing ?
                    //__________________________________________________________
                    if( __collapse(sub) )
                    {
                        assert(false==sub->terminal);
                        stk.merge_back(*(sub->ch));
                        delete sub;
                        continue;
                    }

                    stk.push_back(sub);

                }
                ch.swap_with(stk);
                for(xnode *sub=ch.head;sub;sub=sub->next)
                {
                    sub->parent = node;
                }
                return node;
            }

        }

    }
}
