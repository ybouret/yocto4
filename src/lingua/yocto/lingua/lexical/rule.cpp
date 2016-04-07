#include "yocto/lingua/lexical/rule.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            rule:: ~rule() throw()
            {
            }


            rule:: rule(const string       &rule_id,
                        const pattern::ptr &user_pattern,
                        const action       &user_action,
                        const bool          is_control) :
            next(0),
            prev(0),
            label(rule_id),
            motif(user_pattern),
            apply(user_action),
            ctrl(is_control)
            {
            }


            
        }
    }
    
}
