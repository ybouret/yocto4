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
            actions_(  )
            {
                actions_.push_back(a);
            }
            
            make:: make( const make &other ) :
            rule( other ),
            actions_( other.actions_ )
            {
            }
            
            rule * make:: clone() const 
            {
                return new make( *this );
            }
            
            void make:: apply() 
            {
                for( actions::iterator i = actions_.begin(); i != actions_.end(); ++i )
                {
                    (*i)( *motif );
                }
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
            
            actions *make:: get_actions() throw()
            {
                return & actions_;
            }
            
            
        }
    }
}

