#include "yocto/lingua/lexical/rules.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace lexical
        {
            ////////////////////////////////////////////////////////////////////
            //
            // rule
            //
            ////////////////////////////////////////////////////////////////////
            
            rule:: rule(const string &id,
                        pattern      *p,
                        const action &cb,
                        bool          control) :
            label(id),
            motif(p),
            prev(0),
            next(0),
            proc(cb),
            fctl(control)
            {
            }
            
            rule:: ~rule() throw()
            {
                assert(motif);
                delete motif;
                motif = 0;
            }
            
            rule * rule:: create(const string &id,
                                 pattern      *p,
                                 const action &cb,
                                 bool           control
                                 )
            {
                assert(p!=NULL);
                
                try
                {
                    if( id == "EOF" ) throw exception("rule::create(invalid label=EOF)");
                    return new rule(id,p,cb,control);
                }
                catch(...)
                {
                    delete p;
                    throw;
                }
            }
            
            void rule:: kill( rule *r ) throw()
            {
                assert(r);
                delete r;
            }
            
            bool rule:: produce()
            {
                assert(motif);
                return proc( *motif );
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // rules
            //
            ////////////////////////////////////////////////////////////////////
            r_list::  r_list() throw() : core::list_of<rule>() {}
            r_list:: ~r_list() throw() { delete_with( rule::kill ); }
            
            
        }
        
    }
}
