#ifndef YOCTO_RX_LEX_RULE_INCLUDED
#define YOCTO_RX_LEX_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace lexical
		{
			
#define YOCTO_RX_LEX_RULE_ACTION_ARGS regex::token &

			typedef functor<bool,TL1(YOCTO_RX_LEX_RULE_ACTION_ARGS)> action;
			
			//! pattern smart pointer
			class rule 
			{
			public:
				
				rule               *next;   //!< core::list binary layout
				rule               *prev;   //!< core::list binary layout
				pattern            *motif;  //!< the one and only pattern
				const string        label;  //!< for lexer database
				action              check;  //!< false: doesn't send to parser
				
				//! rule creationg
				/**
				 \param p a valid pattern, handled in case of error
				 \param l associated label
				 */
				static rule *create(  pattern *p, const string &l, const action *a = NULL);
				static void  destroy(  rule *r ) throw();			 
				
				bool keep( YOCTO_RX_LEX_RULE_ACTION_ARGS  ) throw();
				
			private:
				rule( pattern *p, const string &l );
				rule( pattern *p, const string &l, const action &a);
				~rule() throw();
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
			
			
		}
		
	}
	
}

#endif
