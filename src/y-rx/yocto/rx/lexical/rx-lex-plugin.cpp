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
            
            plugin:: plugin(const char     *id,
                            const char     *enter_expr,
                            const char     *leave_expr,
                            const callback &cb,
                            const string   *will_jump) : 
            regex::sublexer(id),
            trigger_( compile(enter_expr,NULL) ),
            enter_( this, & plugin:: on_enter ),
            finish_( cb )
            {
                const action leave_( this, & plugin::on_leave );
                if( will_jump )
                {
                    jump( *will_jump, leave_expr, leave_, NULL );
                }
                else
                {
                    back( leave_expr, leave_, NULL );
                }
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
                finish_( this->data() );
            }
            
            pattern * plugin:: trigger() const
            {
                return trigger_->clone();
            }
            
            
        }
    }
}

