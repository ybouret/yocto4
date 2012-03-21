#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        void sub_lexer:: jump(const string &name, pattern *p, const lexical::action &a  )
        {
            rules_.push_back( lexical::jump::create(p,a,&parent,name) );
        }

        void sub_lexer:: jump(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict )
        {
            rules_.push_back( lexical::jump::create( compile(expr,dict),a,&parent,name) );
        }


        void sub_lexer::jump(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict )
        {
            const string __name(name);
            rules_.push_back( lexical::jump::create( compile(expr,dict),a,&parent,__name) );
        }
        
        
        void sub_lexer:: call(const string &name, pattern *p, const lexical::action &a  )
        {
            rules_.push_back( lexical::call::create(p,a,&parent,name) );
        }
        
        void sub_lexer:: call(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict )
        {
            rules_.push_back( lexical::call::create( compile(expr,dict),a,&parent,name) );
        }
        
        
        void sub_lexer::call(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict )
        {
            const string __name(name);
            rules_.push_back( lexical::call::create( compile(expr,dict),a,&parent,__name) );
        }

        //======================================================================
        // back API
        //======================================================================
        void sub_lexer:: back( pattern *p, const lexical::action &a )
        {
            rules_.push_back( lexical::back::create(p,a,&parent) );
        }
        
        void sub_lexer::  back( const string &expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->back( p, a );
        }
        
        void sub_lexer::  back( const char *expr, const lexical::action &a, pattern_dict *dict  )
        {
            pattern *p = compile(expr,dict);
            this->back( p, a );
        }

        
    }

}

