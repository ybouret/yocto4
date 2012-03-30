#include "yocto/rx/lexical/rule.hpp"

namespace yocto
{
    namespace regex
    {
        namespace lexical
        {
            rule:: ~rule() throw()
            {
                assert(NULL != motif);
                delete motif;
            }
            
            rule:: rule( pattern *p ) :
            next(NULL),
            prev(NULL),
            motif( p )
            {
                assert( NULL != motif );
            }
            
            rule:: rule( const rule &r ) :
            next(NULL),
            prev(NULL),
            motif( r.motif->clone() )           
            {
            }
            
            actions * rule:: get_actions() throw()
            {
                return NULL;
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // make
            //
            ////////////////////////////////////////////////////////////////////
            make:: ~make() throw()
            {
            }
            
            make:: make( pattern *p, const action &a ) :
            rule( p ),
            action_( a )
            {
            }
            
            make:: make( const make &other ) :
            rule( other ),
            action_( other.action_ )
            {
            }
            
            rule * make:: clone() const 
            {
                return new make( *this );
            }
            
            void make:: apply() 
            {
                action_( *motif );
            }
            
            rule * make::create( pattern *p, const action &a )
            {
                try {
                    return new make(p,a);
                } catch (...) {
                    delete p;
                    throw;
                }
            }
            
            
            
        }
    }
}