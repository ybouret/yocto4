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
            
            
            //! a lexical action
            /**
             Do something with a matching token
             \return true => produces a lexeme, false => discard the lexeme
             */
            typedef functor<bool,TL1(const lang::token &)> action;
            
            class rule : public object
            {
            public:
                const string label;    //!< unique label, transmitted to lexeme
                pattern     *motif;    //!< the accepting pattern
                action       deed;     //!< what to do when the motif wins
                const bool   ctrl;     //!< a control rule ?
                rule        *next;     //!< for the list
                rule        *prev;     //!< for the list
                
                virtual ~rule() throw();
                
                static   rule *create(const string &id, pattern *p, const action &cb, const bool flag);
                
            private:
                rule(const string &id, pattern *pp, const action &cb, const bool flag );
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };
            
            typedef core::list_of_cpp<rule> r_list;
            
        }
    }
}

#endif

