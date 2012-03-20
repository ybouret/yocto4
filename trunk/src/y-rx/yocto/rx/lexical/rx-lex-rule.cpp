#include "yocto/rx/lexical/rule.hpp"

namespace yocto
{
    namespace regex
    {
        namespace lexical
        {
            rule:: ~rule() throw()
            {
            }
            
            rule:: rule( pattern *p ) :
            next(NULL),
            prev(NULL),
            motif( p )
            {
            }
            
            rule:: rule( const rule &r ) :
            next(NULL),
            prev(NULL),
            motif( r.motif->clone() )            {
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
            
            
        }
    }
}