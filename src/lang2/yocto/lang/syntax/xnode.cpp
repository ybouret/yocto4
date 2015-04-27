#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            xnode:: ~xnode() throw()
            {
                switch(type)
                {
                    case is_terminal:
                        assert(lx);
                        delete lx;
                        lx = 0;
                        break;

                    case is_internal:
                        assert(ch);
                        ch->clear();
                        object::release1(ch);
                        ch = 0;
                        break;
                }
            }

            xnode::xnode(type_t t) throw() :
            next(0),
            prev(0),
            type(t)
            {
            }

            xnode * xnode:: term(lexeme *l)
            {
                assert(l);
                try
                {
                    xnode *node = new xnode(is_terminal);
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
                xnode *node = new xnode(is_internal);
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

        }

    }

}
