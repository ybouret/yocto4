#include "yocto/lang/syntax/rule.hpp"
#include "yocto/exception.hpp"

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

            xnode::xnode(const rule &r, bool flag, const property ppty):
            next(0),
            prev(0),
            parent(0),
            label(r.label),
            terminal(flag),
            modifier(ppty)
            {
                if(terminal)
                {
                    if(mergeAll==modifier) throw exception("|mergeAll| terminal '%s'", r.label.c_str());
                    if(mergeOne==modifier) throw exception("|mergeOne| terminal '%s'", r.label.c_str());

                }
                else
                {
                    if(jettison==modifier) throw exception("|jettison| leaf '%s'", r.label.c_str());
                    if(univocal==modifier) throw exception("|univocal| leaf '%s'", r.label.c_str());
                }
            }

            xnode * xnode:: term(const rule &r, lexeme *l,const property ppty)
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


            xnode *xnode:: leaf(const rule &r, const property ppty)
            {
                xnode *node = new xnode(r,false,ppty);
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

            const lexeme & xnode::lex() const throw()
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

            const xnode::leaves & xnode::children() const throw()
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

            xnode * xnode::pop_head() throw()
            {
                assert(!terminal);
                assert(ch->size>0);
                xnode *node = ch->pop_front();
                assert(node->parent==this);
                node->parent = NULL;
                return node;
            }

            xnode * xnode::pop_tail() throw()
            {
                assert(!terminal);
                assert(ch->size>0);
                xnode *node = ch->pop_back();
                assert(node->parent==this);
                node->parent = NULL;
                return node;
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
                    if(modifier==standard)
                    {
                        fp << '=';
                        pattern::encode(*lx, fp);
                    }
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


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {



            xnode * xnode:: AST( xnode *node ) throw()
            {
                if(!node) return 0;

                if(node->terminal)
                {
                    //__________________________________________________________
                    //
                    // clear content if not standard
                    //__________________________________________________________
                    if(node->modifier!=standard)
                    {
                        node->lx->clear();
                    }
                    return node;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // loop over nodes -> recursion
                    //__________________________________________________________
                    xnode::leaves stk;
                    node->ch->swap_with(stk);
                    while(stk.size)
                    {
                        xnode *sub = AST(stk.pop_front());
                        if(!sub) continue;
                        assert(sub->parent==node);
                        switch(sub->modifier)
                        {
                                //______________________________________________
                                //
                                // syntax only node
                                //______________________________________________
                            case jettison:
                                delete sub;
                                break;

                                //______________________________________________
                                //
                                // temporary node, merge if possible
                                //______________________________________________
                            case mergeAll:
                                assert(!sub->terminal);
                                while(sub->ch->size)
                                {
                                    node->append(sub->pop_head());
                                }
                                delete sub;
                                break;

                            case mergeOne:
                                assert(!sub->terminal);
                                if(1==sub->ch->size)
                                {
                                    node->append(sub->pop_head());
                                    delete sub;
                                }
                                else
                                {
                                    node->ch->push_back(sub);
                                }
                                break;
                                
                            case standard:
                            case univocal:
                                node->ch->push_back(sub);
                                break;
                        }
                    }
                    
                    return node;
                    
                }
                
            }
            
        }
        
    }
    
}


