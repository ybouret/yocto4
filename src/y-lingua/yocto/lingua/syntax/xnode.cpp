#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/code/cast.hpp"
#include <cstring>

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
                    assert(lex!=NULL);
                    delete lex;
                }
                else
                {
                    while( children.size ) delete children.pop_back();
                }
                memset(wksp,0,sizeof(wksp));
            }
            
         
            xnode:: xnode( const string &label_ref, lexeme *lx, node_property p ) throw():
            label(label_ref),
            prev(0),
            next(0),
            parent(0),
            wksp(),
            lex(      * _cast::trans<lexeme*   ,uint64_t>(wksp) ),
            children( * _cast::trans<child_list,uint64_t>(wksp) ),
            terminal(lx!=0),
            property(p)
            {
                memset(wksp,0,sizeof(wksp));
                assert(lex==0);
                assert(children.size==0);
                
                lex = lx;
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
            
        }
    }
    
}
