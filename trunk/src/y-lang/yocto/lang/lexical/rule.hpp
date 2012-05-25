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
            
            class rule
            {
            public:
                const string    label;   //!< terminal label
                regex::pattern *motif;   //!< accepting pattern
                rule           *next;    //!< for linked list
                rule           *prev;    //!< for linked list
               
                
                static rule * create( const string &id, regex::pattern *p, const action &cb, bool control);
                static void   kill( rule *r ) throw();
                
                bool produce(); //! proc( *motif )
                
            private:
                action proc;
                rule( const string &id, regex::pattern *p, const action &cb,bool control);
                ~rule() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            public:
                 const bool      fctl;    //!< control flag: rule is jump/call/back
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
