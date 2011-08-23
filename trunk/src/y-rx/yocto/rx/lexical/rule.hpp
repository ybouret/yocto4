#ifndef YOCTO_RX_LEXICAL_RULE_INCLUDED
#define YOCTO_RX_LEXICAL_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	
	namespace regex
	{
		
		typedef functor<void,TL1(token)> action;
		
		namespace lexical 
		{
			
			class rule : public object
			{
			public:
				rule *next;
				rule *prev;
				virtual rule *clone() const = 0;
				virtual void  apply( const token & ) = 0;
				virtual ~rule() throw();
				
			protected:
				explicit rule() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
			
			typedef core::meta_list<rule>    r_list;
			

			class make : public rule
			{
			public:
				virtual ~make() throw();
				static make *create( const action & );
				
				virtual rule *clone() const { return make::create( action_ ); }
				virtual void  apply( const token & );
				
			private:
				action action_;
				explicit make( const action & );
				YOCTO_DISABLE_COPY_AND_ASSIGN(make);
			};
			
			
			class call : public rule
			{
			public:
				virtual ~call() throw();
				virtual  rule *clone() const { return NULL; }
				virtual  void  apply( const token & );
				
			private:
				action on_enter_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(call);
			};
			
			
		}
		
	}
	
}

#endif
