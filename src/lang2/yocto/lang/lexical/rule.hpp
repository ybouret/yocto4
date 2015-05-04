#ifndef YOCTO_LANG_LEXICAL_RULE_INCLUDED
#define YOCTO_LANG_LEXICAL_RULE_INCLUDED 1


#include "yocto/lang/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            //! lexical action
            /**
             - if it returns true,  the lexeme is emitted
             - if it returns false, the token is dicarded
             */
            typedef functor<bool,TL1(const token &)> action;

            //! a simple lexical rule
            class rule : public object
            {
            public:
                rule        *next;
                rule        *prev;

                const string label; //!< hard coded label, for lexemes
                pattern     *motif; //!< the matching pattern
                action       apply; //!< what to do in case of success
                const bool   ctrl;  //!< true is control rule

                virtual ~rule() throw();

                static rule *create(const string &l, pattern *p, const action &a, const bool is_control=false);

                const string & key() const throw();

            private:
                explicit rule(const string &l, pattern *p, const action &a, const bool is_ctrl);
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };

            typedef core::list_of_cpp<rule> r_list;
        }
    }
}
#endif
