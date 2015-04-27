#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            xnode:: ~xnode() throw()
            {
                if(is_term)
                {
                    assert(lx);
                    delete lx;
                    lx = 0;
                }
                else
                {
                    assert(ch);
                    ch->clear();
                    object::release1(ch);
                    ch = 0;
                }
            }

            xnode::xnode(bool flag) throw() :
            next(0),
            prev(0),
            is_term(flag)
            {
            }

            xnode * xnode:: term(lexeme *l)
            {
                assert(l);
                try
                {
                    xnode *node = new xnode(true);
                    node->lx    = l;
                    return node;
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }


            xnode *xnode:: leaf()
            {
                xnode *node = new xnode(false);
                try
                {
                    node->ch = new (object::acquire1<leaves>()) leaves();
                    return node;
                }
                catch(...)
                {
                    delete node;
                    throw;
                }
            }


            lexeme & xnode::lex() const
            {
                assert(is_term);
                assert(lx);
                return *lx;
            }


        }

    }

}

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            void    xnode:: append(xnode *node) throw()
            {
                assert(node);
                assert(!is_term);
                assert(NULL==node->parent);
                node->parent = this;
                ch->push_back(node);
            }
        }
    }
}


