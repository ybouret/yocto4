#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
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
