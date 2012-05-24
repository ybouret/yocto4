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
                const string    label; //!< terminal label
                regex::pattern *motif; //!< accepting pattern
                rule           *next;  //!< for linked list
                rule           *prev;  //!< for linked list
                
                static rule * create( const string &id, regex::pattern *p, const action &cb );
                static void   kill( rule *r ) throw();
                
            private:
                action proc;
                rule( const string &id, regex::pattern *p, const action &cb);
                ~rule() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
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
