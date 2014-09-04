#include "yocto/lang/syntax/xnode.hpp"

#include <cstring>

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            void xnode:: clr() throw()
            {
                memset(wksp,0,sizeof(wksp));
            }
            
            xnode:: ~xnode() throw()
            {
                assert(NULL==parent);
                assert(NULL==next);
                assert(NULL==prev);
                if(terminal)
                {
                    lexeme *lx = lxm();
                    if(lx) delete lx;
                }
                else
                {
                    child_list &ch = children();
                    while(ch.size)
                    {
                        xnode *node  = ch.pop_back();
                        node->parent = NULL;
                        delete node;
                    }
                }
                clr();
            }
            
            xnode::child_list & xnode::children() throw()
            {
                return *(child_list *) &wksp[0];
            }
            
            const xnode::child_list & xnode::children() const throw()
            {
                return *(child_list *) &wksp[0];
            }
            
            
            lexeme *         & xnode:: lxm() throw()
            {
                return *(lexeme **) &wksp[0];
            }
            
            const lexeme *   & xnode:: lxm() const throw()
            {
                return *(const lexeme **) &wksp[0];
            }
            
            
            xnode:: xnode(const string &rule_label, lexeme *lx) throw():
            label(rule_label),
            next(0),
            prev(0),
            parent(0),
            terminal(true)
            {
                assert(lx);
                clr();
                lxm() = lx;
            }
            
            
            xnode:: xnode(const string &rule_label) throw():
            label(rule_label),
            next(0),
            prev(0),
            parent(0),
            terminal(false)
            {
                clr();
            }
            
            
            
            
            xnode * xnode:: create( const string &rule_label, lexeme *lx )
            {
                try
                {
                    return new xnode(rule_label, lx);
                }
                catch(...)
                {
                    delete lx;
                    throw;
                }
            }
            
            
            xnode * xnode:: create( const string &rule_label )
            {
                return new xnode(rule_label);
            }
            
            void xnode:: add( xnode *child ) throw()
            {
                assert(0!=child);
                assert(0==child->parent);
                assert(!terminal);
                children().push_back(child);
                child->parent = this;
            }
            
            void xnode:: restore( lexer &Lexer, xnode *node ) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    Lexer.unget(node->lxm());
                    node->lxm() = NULL;
                }
                else
                {
                    child_list &ch = node->children();
                    while( ch.size )
                    {
                        xnode *sub  = ch.pop_back();
                        sub->parent = NULL;
                        xnode::restore(Lexer,sub);
                    }
                }
                delete node;
            }

           
            
        }
        
    }
    
}
