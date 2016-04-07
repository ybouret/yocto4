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
             */
            typedef functor<bool,TL1(const token&)> action;

            class rule : public object
            {
            public:
                rule              *next;   //!< for list
                rule              *prev;   //!< for list
                const string       label;  //!< identifier
                const pattern     *motif;  //!< the matcher
                const action       apply;  //!< what to do if win
                const bool         ctrl;   //!< is it a control rule ?

                virtual ~rule() throw();

                static rule *create(const string &id,
                                    pattern      *p,
                                    const action &a,
                                    const bool    is_control);


            private:
                explicit rule(const string &id,
                              pattern      *p,
                              const action &a,
                              const bool    is_control
                              );
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };
            
            typedef core::list_of_cpp<rule> r_list;
        }

    }
}

#endif
