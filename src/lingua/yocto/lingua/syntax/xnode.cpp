#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/code/cast.hpp"
#include <cstring>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            
            xnode::child_list::  child_list() throw() {}
            xnode::child_list:: ~child_list() throw()
            {
                while(size) delete pop_back();
            }
            
            xnode:: ~xnode() throw()
            {
                if(terminal)
                {
                    delete lex();
                }
                else
                {
                    while( children().size ) delete children().pop_back();
                }
                memset(wksp,0,sizeof(wksp));
            }
            
            xnode::child_list & xnode:: children() throw()
            {
                assert(!terminal);
                return * _cast::trans<child_list,uint64_t>(wksp);
            }
            
            const xnode::child_list & xnode:: children() const throw()
            {
                assert(!terminal);
                return * _cast::trans<child_list,uint64_t>((uint64_t*)wksp);
            }
            
            lexeme *& xnode:: lex() throw()
            {
                assert(terminal);
                return * _cast::trans<lexeme*,uint64_t>(wksp);
            }
            
            lexeme * const & xnode:: lex() const throw()
            {
                assert(terminal);
                return * _cast::trans<lexeme*,uint64_t>((uint64_t *)wksp);
            }
            
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
            xnode:: xnode(const string &label_ref,
                          lexeme       *lx,
                          node_property p ) throw():
            label(label_ref),
            prev(0),
            next(0),
            parent(0),
            wksp(),
            terminal(lx!=0),
            property(p)
            {
                memset(wksp,0,sizeof(wksp));
                if(lx)
                    lex() = lx;
            }
            
            xnode * xnode:: create( const string &label_ref, lexeme *lx, node_property p )
            {
                try
                {
                    return new xnode(label_ref,lx,p);
                }
                catch(...)
                {
                    if(lx) delete lx;
                    throw;
                }
                
            }
            
            xnode * xnode::unlink() throw()
            {
                assert(parent);
                assert(! parent->terminal );
                assert(  parent->children().owns(this) );
                xnode *self = parent->children().unlink(this);
                assert(this==self);
                self->parent = 0;
                return this;
            }
            
            
            
            void xnode:: restore( lexer &Lexer, xnode *node ) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    Lexer.unget(node->lex());
                    node->lex() = 0;
                }
                else
                {
                    child_list &ch = node->children();
                    while( ch.size ) xnode::restore(Lexer, ch.pop_back() );
                }
                delete node;
            }
            
        }
    }
    
}
