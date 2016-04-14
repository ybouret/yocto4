#ifndef YOCTO_LINGUA_LEXICAL_RULE_INCLUDED
#define YOCTO_LINGUA_LEXICAL_RULE_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            //! lexical action
            /**
             - if it returns true,  the lexeme is emitted
             - if it returns false, the token is dicarded
             a control rule MUST return false
             */
            typedef functor<bool,TL1(const token&)> action;

            class rule : public object
            {
            public:
                rule              *next;   //!< for list
                rule              *prev;   //!< for list
                const string       label;  //!< identifier
                const pattern::ptr motif;  //!< the matcher
                action             apply;  //!< what to do if win
                const bool         ctrl;   //!< is it a control rule ?

                virtual ~rule() throw();
                
                explicit rule(const string       &rule_id,
                              const pattern::ptr &user_pattern,
                              const action       &user_action,
                              const bool          is_control
                              );
                


            private:

                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };
            
            typedef core::list_of_cpp<rule> r_list;
        }

    }
}

#endif
