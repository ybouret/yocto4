#ifndef YOCTO_RX_LEX_RULE_INCLUDED
#define YOCTO_RX_LEX_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    namespace regex
    {
        
        namespace lexical
        {
            
            typedef functor<void,TL1(const token &)> action;
            
            class rule : public object
            {
            public:
                rule             *next;
                rule             *prev;
                auto_ptr<pattern> motif;
                
                virtual rule *clone() const = 0;
                virtual void  apply() = 0;
                virtual ~rule() throw();
                
            protected:
                explicit rule( pattern *p );
                rule( const rule & );
                
            private:
                YOCTO_DISABLE_ASSIGN(rule);
            };
            
            class make : public rule
            {
            public:
                explicit make( pattern *p, const action &a);
                virtual ~make() throw();
                
                virtual rule *clone() const;
                virtual void  apply();
                
            private:
                action action_;
                make( const make & );
                YOCTO_DISABLE_ASSIGN(make);
            };
            
        }
        
    }
}
#endif

