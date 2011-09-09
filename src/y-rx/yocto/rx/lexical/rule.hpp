#ifndef YOCTO_RX_LEXICAL_RULE_INCLUDED
#define YOCTO_RX_LEXICAL_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		
		namespace lexical
		{
			typedef functor<void,TL1(const token & )> action;
#define YLEX_CB(method,args) void method( const regex::token &args )
			
			//! basic rule
			class rule : public object
			{
			public:
				rule *next;
				rule *prev;
				virtual ~rule() throw();
				virtual  rule *clone() const = 0;
				virtual void   apply( const token & ) = 0;
				
			protected:
				explicit rule() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
			typedef core::meta_list<rule> r_list;
			
			//! make something 
			class make : public rule
			{
			public:
				virtual ~make() throw();
				virtual  rule * clone() const;
				virtual void    apply( const token & );
				
				static  rule   *create( const action &a );
				
			private:
				explicit make( const action &a );
				action todo_;				
				YOCTO_DISABLE_COPY_AND_ASSIGN(make);
			};
			
			class engine;
			
			class stop : public rule
			{
			public:
				virtual ~stop() throw();
				virtual  rule * clone() const;
				virtual void    apply( const token & );
				
				static rule *create( engine &e, const action &a);
				
			private:
				engine &engine_;
				action  onStop_;
				explicit stop( engine &e, const action &a );
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(stop);
				
			};
			
		}
	}
	
}

#endif
