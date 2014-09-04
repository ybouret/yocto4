#include "yocto/lang/syntax/xnode.hpp"

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
                    delete lxm();
                }
                else
                {
                    children().auto_delete();
                }
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
                lxm() = lx;
            }
            
            
            xnode:: xnode(const string &rule_label) throw():
            label(rule_label),
            next(0),
            prev(0),
            parent(0),
            terminal(false)
            {
                children().reset();
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
            
            
        }
        
    }
    
}
