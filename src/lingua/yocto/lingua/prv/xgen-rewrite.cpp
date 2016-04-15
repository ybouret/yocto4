#include "yocto/lingua/prv/xgen.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            static inline xnode * __reformat( xnode *node )
            {
                assert("SUB"==node->origin->label);
                assert(false==node->terminal);
                assert(node->ch->size>=2);
                assert(NULL==node->parent);
                assert(NULL==node->next);
                assert(NULL==node->prev);

                xlist &ch = *(node->ch);
                if("ALT"==ch.tail->origin->label)
                {
                    //__________________________________________________________
                    //
                    // fusion ALTs
                    //__________________________________________________________
                    {
                        // finding fist ALT
                        xnode *first = ch.head->next;
                        while("ALT"!=first->origin->label)
                        {
                            first=first->next;
                            assert(NULL!=first);
                        }

                        // fusion other
                        xlist tmp;
                        while(ch.tail!=first)
                        {
                            tmp.push_front(ch.pop_back());
                        }

                        while(tmp.size>0)
                        {
                            xnode *alt = tmp.pop_front();
                            assert("ALT"==alt->origin->label);
                            assert(false==alt->terminal);
                            assert(1==alt->ch->size);
                            xnode *arg = alt->ch->pop_back();
                            first->ch->push_back(arg);
                            arg->parent = first;
                            delete alt;
                        }

                        assert(first==ch.tail);
                    }

                    //__________________________________________________________
                    //
                    // rotation with noSingle flag for sub...
                    //__________________________________________________________
                    xnode *alt = ch.pop_back();
                    alt->parent = 0;

                    if(node->ch->size!=1)
                    {
                        alt->ch->push_front(node);
                        node->parent = alt;
                    }
                    else
                    {
                        xnode *sub = node->ch->pop_back();
                        delete node;
                        alt->ch->push_front(sub);
                        sub->parent = alt;
                    }

                    return alt;
                }
                else
                {
                    return node;
                }

            }

            xnode * xgen:: rewrite(xnode *tree)
            {
                assert(tree);
                if(tree->terminal)
                {
                    return tree;
                }
                else
                {
                    xlist &tch = *(tree->ch);
                    xlist  stk;

                    while(tch.size>0)
                    {
                        xnode *node  = tch.pop_front();
                        node->parent = 0;
                        node = rewrite(node);
                        if("SUB"==node->origin->label)
                        {
                            node = __reformat(node);
                        }

                        node->parent = tree;
                        stk.push_back(node);
                    }

                    tch.swap_with(stk);


                    return tree;
                }
            }
        }
        
    }
}
