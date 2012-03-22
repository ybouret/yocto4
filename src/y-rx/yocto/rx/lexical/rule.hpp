#ifndef YOCTO_RX_LEX_RULE_INCLUDED
#define YOCTO_RX_LEX_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    namespace regex
    {
        
        class     lexer;
        namespace lexical
        {
            
            typedef functor<void,TL1(const token &)> action;
            
            //! a rule: motif => apply
            class rule : public object
            {
            public:
                rule             *next;
                rule             *prev;
                pattern          *motif;
                
                virtual rule *clone() const = 0;
                virtual void  apply() = 0;
                virtual ~rule() throw();
                
            protected:
                explicit rule( pattern *p );
                rule( const rule & );
                
            private:
                YOCTO_DISABLE_ASSIGN(rule);
            };
            
            //! list of rules
            typedef core::meta_list<rule> rules;
            
            //! motif => apply action
            class make : public rule
            {
            public:
                virtual ~make() throw();
              
                static  rule *create( pattern *p, const action &a );
               
                virtual rule *clone() const;
                virtual void  apply();
                
            private:
                explicit make( pattern *p, const action &a);
                action action_;
                make( const make & );
                YOCTO_DISABLE_ASSIGN(make);
            };
            
            
            //! motif => apply action and jump to new sub_lexer 'name'
            class jump : public rule
            {
            public:
                virtual ~jump() throw();
                
                static  rule *create( pattern *p, const action &a, lexer **ppLx, const string &name );
                
                virtual rule *clone() const;
                virtual void  apply(); 
                
                
            private:
                explicit jump( pattern *p, const action &a, lexer **ppLx, const string &name );
                jump( const jump & );
                action        on_enter;
                lexer       **pp_lexer;
                const string  sub_name;
                
                YOCTO_DISABLE_ASSIGN(jump);
            };
            
            //! motif => apply action and call new sub_lexer 'name'
            class call : public rule
            {
            public:
                virtual ~call() throw();
                
                static  rule *create( pattern *p, const action &a, lexer **ppLx, const string &name );
                
                virtual rule *clone() const;
                virtual void  apply(); 
                
                
            private:
                explicit call( pattern *p, const action &a, lexer **ppLx, const string &name );
                call( const call & );
                action        on_enter;
                lexer       **pp_lexer;
                const string  sub_name;
                
                YOCTO_DISABLE_ASSIGN(call);
            };
            
            //! motif => apply action and back
            class back : public rule
            {
            public:
                virtual ~back() throw();
                
                static  rule *create( pattern *p, const action &a, lexer **ppLx);
            
                virtual rule *clone() const;
                virtual void  apply(); 
                
            private:
                explicit back( pattern *p, const action &a, lexer **ppLx );
                back( const back & );
                action  on_leave;
                lexer **pp_lexer;
                YOCTO_DISABLE_ASSIGN(back);
            };

            
            
        }
        
    }
}
#endif

