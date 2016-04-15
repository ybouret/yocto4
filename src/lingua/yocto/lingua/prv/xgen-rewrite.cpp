#include "yocto/lingua/prv/xgen.hpp"
#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


            static inline
            void reformat( xnode *sub ) throw()
            {
                assert(sub!=NULL);
                assert("SUB"==sub->origin->label);
                xlist &sub_ch = *(sub->ch);
                assert(sub_ch.size>=2);
                assert("ALT"==sub_ch.tail->origin->label);

                //-- find first ALT
                xnode *first_alt = sub_ch.head;
                while("ALT"!=first_alt->origin->label)
                {
                    first_alt = first_alt->next;
                    assert(first_alt);
                }

                {
                    xlist tmp;
                    while(sub_ch.tail!=first_alt)
                    {
                        tmp.push_front(sub_ch.pop_back());
                    }
                    while(tmp.size)
                    {
                        xnode *alt = tmp.pop_front();
                        assert("ALT"==alt->origin->label);
                        assert(1==alt->ch->size);
                        xnode *arg = alt->ch->pop_back();
                        first_alt->push_back(arg);
                        arg->parent = first_alt;
                        delete alt;
                    }
                }

            }

            xnode * xgen:: rewrite(xnode *tree) throw()
            {
                assert(tree);
                if(tree->terminal)
                {
                    return tree;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // post-process
                    //__________________________________________________________
                    xlist &ch  = *(tree->ch);
                    xlist  stk;
                    while(ch.size>0)
                    {
                        xnode *node = ch.pop_front(); assert(node->parent==tree);
                        node = rewrite(node);
                        node->parent = tree;
                        if("SUB"==node->origin->label)
                        {
                            assert(false==node->terminal);
                            assert(node->ch->size>=2);
                            if(node->ch->tail->origin->label=="ALT")
                            {
                                std::cerr << "found SUB/ALT" << std::endl;
                                reformat(node);
                            }
                        }

                        stk.push_back(node);
                    }
                    ch.swap_with(stk);
                    return tree;
                }
            }
        }
        
    }
}
