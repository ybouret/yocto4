#include "yocto/lang/syntax/rule.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            xnode:: ~xnode() throw()
            {
                if(terminal)
                {
                    if(lx)
                    {
                        delete lx;
                        lx = 0;
                    }
                }
                else
                {
                    assert(ch);
                    ch->clear();
                    object::release1(ch);
                    ch = 0;
                }
            }

            xnode::xnode(const rule &r, bool flag, const property ppty) throw() :
            next(0),
            prev(0),
            parent(0),
            label(r.label),
            terminal(flag),
            modifier(ppty)
            {
            }

            xnode * xnode:: term(const rule &r, lexeme *l, const property ppty)
            {
                assert(l);
                try
                {
                    xnode *node = new xnode(r,true,ppty);
                    node->lx    = l;
                    return node;
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }


            xnode *xnode:: leaf(const rule &r)
            {
                xnode *node = new xnode(r,false,variable);
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


            lexeme & xnode::lex() throw()
            {
                assert(terminal);
                assert(lx);
                return *lx;
            }

            xnode::leaves & xnode::children() throw()
            {
                assert(!terminal);
                assert(ch);
                return *ch;
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
                assert(!terminal);
                assert(NULL==node->parent);
                node->parent = this;
                ch->push_back(node);
            }


            void xnode:: restore(xnode *node, l_list &lexemes) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    lexemes.push_front(node->lx);
                    node->lx = 0;
                }
                else
                {
                    xnode::leaves &nodes = node->children();
                    while(nodes.size>0)
                    {
                        restore( nodes.pop_back(), lexemes );
                    }
                }
                delete node;
            }


        }
    }
}

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            void xnode:: viz( ios::ostream &fp ) const
            {
                fp.viz(this);
                if(terminal)
                {
                    assert(lx);
                    fp( "[shape=box,label=\"");
                    fp << label;
                    fp << '=';
                    pattern::encode(*lx, fp);
                    fp("\"];\n");
                }
                else
                {
                    assert(ch);
                    fp("[shape=egg,label=\"");
                    fp << label;
                    fp("\"];\n");
                    for(const xnode *node=ch->head;node;node=node->next)
                    {
                        node->viz(fp);
                        fp.viz(this); fp(" -> "); fp.viz(node); fp("\n");
                    }
                }
            }

            void xnode:: graphivz( const string &filename ) const
            {
                ios::ocstream fp(filename,false);
                fp("digraph G {\n");
                
                viz(fp);
                
                fp("}\n");
            }
            
            
        }
    }
}


