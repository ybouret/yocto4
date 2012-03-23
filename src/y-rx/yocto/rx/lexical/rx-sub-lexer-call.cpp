#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        
        //======================================================================
        // call API
        //======================================================================
        void sublexer:: call(const string &name, pattern *p, const lexical::action &a  )
        {
            rules_.push_back( lexical::call::create(p,a,&parent,name) );
        }
        
        void sublexer:: call(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict )
        {
            pattern     *  p = compile(expr,dict);
            this->call( name, p, a );
        }
        
        
        void sublexer::call(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict )
        {
            const string   n(name);
            pattern     *  p = compile(expr,dict);
            this->call( n, p, a );
        }

    }
}
