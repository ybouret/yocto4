#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        sub_lexer:: ~sub_lexer() throw()
        {
            
        }
        
        sub_lexer:: sub_lexer( const string &id ) :
        name( id ),
        rules_()
        {
        }
        
        sub_lexer:: sub_lexer( const char *id ) :
        name( id ),
        rules_()
        {
        }
        
        const string & sub_lexer:: key() const throw() { return name; }
        
        void sub_lexer:: reset() throw()
        {
            for( lexical::rule *r = rules_.head; r; r=r->next )
            {
                r->motif->clear();
            }
        }
        
        //======================================================================
        // Make API
        //======================================================================
        void sub_lexer:: make( pattern *p, const lexical::action &a )
        {
            rules_.push_back( lexical::make::create(p,a) );
        }
        
        void sub_lexer::  make( const string &expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->make( p, a );
        }

        void sub_lexer::  make( const char *expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->make( p, a );
        }
    }
}