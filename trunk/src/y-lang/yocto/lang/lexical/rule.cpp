#include "yocto/lang/lexical/rule.hpp"

namespace yocto 
{
    namespace lang
    {
        
        namespace lexical
        {

            ////////////////////////////////////////////////////////////////////
            //
            //
            //
            ////////////////////////////////////////////////////////////////////

            rule:: ~rule() throw()
            {
                assert(motif!=NULL);
                delete motif;
            }
            
            
            rule:: rule( const string &id, regex::pattern *p, const action &cb ) :
            name(id),
            motif(p),
            next(0),
            prev(0),
            proc(cb)
            {
            }
            
            void rule:: kill( rule *r ) throw() { assert(r); r->~rule(); object::release1(r); }
            
            rule * rule:: create(const string &id, regex::pattern *p, const action &cb)
            {
                assert(p);
                rule *r = object::acquire1<rule>();
                try
                {
                    new (r) rule(id,p,cb);
                }
                catch(...)
                {
                    object::release1(r);
                    delete p;
                    throw;
                }
                return r;
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            //
            //
            ////////////////////////////////////////////////////////////////////
            rules:: ~rules() throw() { delete_with( rule::kill ); }
            
            rules:: rules() throw() {}
        }
    }
}