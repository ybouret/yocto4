#ifndef YOCTO_LANG_LEXICAL_RULE_INCLUDED
#define YOCTO_LANG_LEXICAL_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace lexical
        {
            
            typedef functor<bool,TL1(const regex::token)> action;
            
            //! generic rule
            class rule : public object
            {
            public:
                const string    label;   //!< terminal label
                regex::pattern *motif;   //!< accepting pattern
                rule           *next;    //!< for linked list
                rule           *prev;    //!< for linked list
                
                
                //! create a rule
                /**
                 \param id will be rule label
                 \param p  will be rule motif: taken care of in case of error
                 \param cb callback when motif is selected
                 \param control will be the special fctl flag
                 the id cannot be EOF
                 */
                static rule * create( const string &id, regex::pattern *p, const action &cb, bool control);
                static void   kill( rule *r ) throw();
                
                //! return proc( *motif )
                /**
                 when motif is selected, call this->proc( *motif );
                 */
                bool produce();
                
            private:
                action proc;
                rule( const string &id, regex::pattern *p, const action &cb,bool control);
                ~rule() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            public:
                const bool      fctl;    //!< control flag: true if rule is jump/call/back
            };
            
            class rules : public core::list_of<rule>
            {
            public:
                explicit rules() throw();
                virtual ~rules() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rules);
            };
        }
        
    }
}

#endif
