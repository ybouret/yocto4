#include "yocto/rx/lexer.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace regex
    {
        
        bool sublexer:: could_append(  pattern *p, const lexical::action &a )
        {
            try
            {
                //--------------------------------------------------------------
                //
                // Look for the same pattern
                //
                //--------------------------------------------------------------
                for( lexical::rule *r = rules_.head; r != NULL; r = r->next )
                {
                    const pattern *q = r->motif;
                    if( pattern::is_same( *p, *q) )
                    {
                        //------------------------------------------------------
                        //-- same pattern is found: does it have actions ?
                        //------------------------------------------------------
                        lexical::actions *pActions = r->get_actions();
                        if( !pActions )
                        {
                            //--------------------------------------------------
                            //-- this is not a possible !
                            //--------------------------------------------------
                            throw exception("sublexer:: no possible multiple actions!");
                        }
                        
                        //------------------------------------------------------
                        //-- ok, increase the actions list (may throw)
                        //------------------------------------------------------
                        pActions->push_back(a);
                        
                        //------------------------------------------------------
                        //-- success, no throw
                        //------------------------------------------------------
                        delete p;
                        return true;
                    }
                }
            }
            catch(...)
            {
                delete p;
                throw;
            }
            
            return false;
        }
        
        //======================================================================
        // Make API
        //======================================================================
        void sublexer:: make( pattern *p, const lexical::action &a )
        {
            if( !could_append(p,a) )
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
