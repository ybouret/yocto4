#include "yocto/rx/lexical/plugin.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            plugin:: ~plugin() throw()
            {
            }
            
            plugin:: plugin( const char *id,
                            const char *enter_expr,
                            const char *leave_expr ) : 
            regex::sublexer(id),
            trigger_( compile(enter_expr,NULL) ),
            enter_( this, & plugin:: on_enter )
            {
                const action leave_( this, & plugin::on_leave );
                back( leave_expr, leave_, NULL );
            }
            
            const action & plugin:: on_call() const throw()
            {
                return enter_;
            }
            
            void plugin:: on_enter( const token & )
            {
                this->enter();
            }
            
            void plugin:: on_leave( const token & )
            {
                this->leave();
            }
            
            pattern * plugin:: trigger() const
            {
                return trigger_->clone();
            }
                       
            
        }
    }
}

