#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
    namespace regex
    {
        //======================================================================
        // jump API
        //======================================================================
        void sublexer:: jump(const string &name, pattern *p, const lexical::action &a  )
        {
            rules_.push_back( lexical::jump::create(p,a,&parent,name) );
        }
        
        void sublexer:: jump(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict )
        {
            pattern *p = compile(expr,dict);
            this->jump( name, p, a );
        }
        
        
        void sublexer::jump(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict )
        {
            const string   n(name);
            pattern     *  p = compile(expr,dict);
            this->jump( n, p, a );
        }

    }
}
