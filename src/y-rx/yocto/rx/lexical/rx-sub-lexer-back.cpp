#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        
        //======================================================================
        // back API
        //======================================================================
        void sublexer:: back( pattern *p, const lexical::action &a )
        {
            rules_.push_back( lexical::back::create(p,a,&parent) );
        }
        
        void sublexer::  back( const string &expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->back( p, a );
        }
        
        void sublexer::  back( const char *expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->back( p, a );
        }
        
        
    }
    
}

