#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        //======================================================================
        // Join API
        //======================================================================
        void sublexer:: join( pattern *p, const lexical::action &a )
        {
            if( rules_.tail )
            {
                lexical::rule *r = rules_.pop_back();
                try
                {
                    make( p, a );
                    rules_.push_back(r);
                }
                catch(...)
                {
                    rules_.push_back(r);
                    throw;
                }
                
            }
            else
                make( p, a );
        }
        
        void sublexer::  join( const string &expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->join( p, a );
        }
        
        void sublexer::  join( const char *expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->join( p, a );
        }
        
    }
}
