#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
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
                    delete ch;
                    ch = 0;
                }

            }



            void xnode::push_back( xnode *node ) throw()
            {
                assert(node);
                assert(!terminal);
                assert(ch);
                ch->push_back(node);
                node->parent = this;
            }

            xnode * xnode::pop_back() throw()
            {
                assert(!terminal);
                assert(ch);
                assert(ch->size>0);
                xnode *node  = ch->pop_back();
                node->parent = 0;
                return node;
            }

            xnode *xnode::pop_front() throw()
            {
                assert(!terminal);
                assert(ch);
                assert(ch->size>0);
                xnode *node  = ch->pop_front();
                node->parent = 0;
                return node;
            }

            size_t xnode::size() const throw()
            {
                return terminal ? 0 : ch->size;
            }

            const xnode *xnode::tail() const throw()
            {
                assert(!terminal);
                return ch->tail;
            }

            const xnode *xnode::head() const throw()
            {
                assert(!terminal);
                return ch->head;
            }

        }
    }
}


#include "yocto/lingua/syntax/term.hpp"
namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            const string & xnode::label() const throw()
            {
                assert(origin);
                return origin->label;
            }
            
            
            xnode * xnode:: create( const rule &r, lexeme *l )
            {
                assert(terminal::UUID==r.uuid);
                assert(l!=0);
                try
                {
                    xnode *node = object::acquire1<xnode>();
                    (bool&)(node->terminal) = true;
                    node->origin = &r;
                    node->lx     =  l;
                    assert(node->origin->label==node->lx->label);
                    return node;
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }

            xnode * xnode::create(const rule &r)
            {
                assert(terminal::UUID!=r.uuid);
                xnode *node = object::acquire1<xnode>();
                try
                {
                    node->ch = new xlist();
                    (bool&)(node->terminal) = false;
                    node->origin = &r;
                }
                catch(...)
                {
                    object::release1<xnode>(node);
                    throw;
                }
                return node;
            }

            void xnode:: back_to(lexer &lxr, xnode *node) throw()
            {
                if(node)
                {
                    if(node->terminal)
                    {
                        assert(node->lx);
                        lxr.unget(node->lx);
                        node->lx = 0;
                    }
                    else
                    {
                        while(node->ch->size)
                        {
                            back_to(lxr,node->ch->pop_back());
                        }
                    }
                    delete node;
                }
            }

        }
    }
}

#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/lingua/syntax/joker.hpp"
#include "yocto/lingua/syntax/compound.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void xnode:: viz( ios::ostream &fp ) const
            {
                const char *shape = "box";
                switch(origin->uuid)
                {
                    case at_least::UUID:  shape = "trapezium"; break;
                    case optional::UUID:  shape = "circle";    break;
                    case aggregate::UUID: shape = "house";    break;
                    default:
                        break;
                }
                assert(shape);

                if(terminal)
                {
                    fp.viz(this);
                    fp << "[label=\"";
                    const string content =  lx->to_string();
                    ios::graphviz_encode(content,fp);
                    fp << "\",shape=" << shape;
                    fp << "];\n";
                }
                else
                {
                    fp.viz(this);
                    fp << "[label=\"";
                    ios::graphviz_encode(label(), fp);
                    fp << "\",shape=" << shape;
                    fp << "];\n";
                    for(const xnode *sub = ch->head; sub; sub=sub->next)
                    {
                        sub->viz(fp);
                        fp.viz(this); fp << "->"; fp.viz(sub); fp << ";\n";
                    }
                }
            }

            void xnode:: graphviz(const string &filename) const
            {
                ios::wcstream fp(filename);
                fp << "digraph G {\n";
                viz(fp);
                fp << "}\n";
            }

        }

    }

}
