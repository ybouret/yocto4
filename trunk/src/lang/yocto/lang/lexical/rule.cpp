#include "yocto/lang/lexical/rule.hpp"
#include "yocto/exception.hpp"

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
            
            
            rule:: rule( const string &id, regex::pattern *p, const action &cb, bool control_flag ) :
            label(id),
            motif(p),
            next(0),
            prev(0),
            proc(cb),
            fctl(control_flag)
            {
            }
            
            void rule:: kill( rule *r ) throw() { assert(r); r->~rule(); object::release1(r); }
            
            rule * rule:: create(const string &id, regex::pattern *p, const action &cb, bool control_flag)
            {
                assert(p);
                if( id == "EOF" )
                    throw exception("EOF is not allowed for syntax::rule");
                rule *r = object::acquire1<rule>();
                try
                {
                    new (r) rule(id,p,cb,control_flag);
                }
                catch(...)
                {
                    object::release1(r);
                    delete p;
                    throw;
                }
                return r;
            }
            
            bool rule:: produce()
            {
                return proc( *motif );
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

