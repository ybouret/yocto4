#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        sublexer:: ~sublexer() throw()
        {
            
        }
        
        sublexer:: sublexer( const string &id, lexer *lx) :
        name( id ),
        rules_(),
        parent( lx )
        {
        }
        
        sublexer:: sublexer( const char *id, lexer *lx ) :
        name( id ),
        rules_(),
        parent( lx )
        {
        }
        
        void sublexer:: attach( lexer *lx ) throw()
        {
            parent = lx;
        }
        
        bool sublexer:: is_attached() const throw()
        {
            return parent != NULL;
        }
        
        
        const string & sublexer:: key() const throw() { return name; }
        
        void sublexer:: reset() throw()
        {
            for( lexical::rule *r = rules_.head; r; r=r->next )
            {
                r->motif->clear();
            }
        }
     
        void sublexer:: discard( const token & ) throw() {}

        
    }
}