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

        
        const string & sublexer:: key() const throw() { return name; }
        
        void sublexer:: reset() throw()
        {
            for( lexical::rule *r = rules_.head; r; r=r->next )
            {
                r->motif->clear();
            }
        }
        
        //======================================================================
        // Make API
        //======================================================================
        void sublexer:: make( pattern *p, const lexical::action &a )
        {
            rules_.push_back( lexical::make::create(p,a) );
        }
        
        void sublexer::  make( const string &expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->make( p, a );
        }

        void sublexer::  make( const char *expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->make( p, a );
        }
    }
}