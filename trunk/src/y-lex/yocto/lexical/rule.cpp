#include "yocto/lexical/rule.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto 
{
    namespace lexical
    {
        rule:: ~rule() throw()
        {
            assert(motif!=NULL);
            delete motif;
        }
        
        rule:: rule( const string &l, regex::pattern *p, const action *a) :
        label(l),
        motif(p),
        next(0),
        prev(0),
        process( a ? *a : action( this, & rule::keep ) )
        {
        }
        
        rule * rule:: make( const string &l, regex::pattern *p, const action *a )
        {
            assert(0!=p);
            rule *r = object::acquire1<rule>();
            try 
            {
                new (r) rule(l,p,a);
            }
            catch(...)
            {
                object::release1<rule>(r);
                delete p;
                throw;
            }
            return r;
        }
        
        void  rule:: kill(rule *r) throw()
        {
            assert(r!=NULL);
            r->~rule();
            object::release1<rule>(r);
        }
        
#if 0
        rule * rule:: make( const char *l, regex::pattern *p )
        {
            auto_ptr<regex::pattern> P(p);
            const string             L(l);
            return make( L, P.yield() );
        }
        
        rule * rule:: make( const string &l, const string &expr, regex::pattern_dict *dict)
        {
            regex::pattern *p = regex::compile(expr,dict);
            return make(l,p);
        }
        
        rule *rule:: make( const char   *l, const char   *expr, regex::pattern_dict *dict )
        {
            const string    L(l);
            regex::pattern *p = regex::compile(expr,dict);
            return make(L,p);
        }
#endif
        
        rules:: ~rules() throw() { delete_with( rule::kill ); }
        rules:: rules() throw()  {}
        
        
    }
}
