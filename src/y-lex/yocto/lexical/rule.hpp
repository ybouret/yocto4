#ifndef YOCTO_LEXICAL_RULE_INCLUDED
#define YOCTO_LEXICAL_RULE_INCLUDED 1

#include "yocto/rx/pattern/dict.hpp"
#include "yocto/functor.hpp"

namespace yocto 
{
    namespace lexical
    {
        
        typedef functor<bool,TL1(const regex::token &)> action;
        
        class rule 
        {
        public:
            const string     label;
            regex::pattern  *motif;
            rule            *next;
            rule            *prev;
            action           process;
            
            static rule *make( const string &l, regex::pattern *p, const action *a=NULL);
            static void  kill(rule *r) throw();
            
            
        private:
            bool keep( const regex::token &) throw();
            rule( const string &l, regex::pattern *p, const action *a);
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

#endif
