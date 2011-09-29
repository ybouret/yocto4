#ifndef YOCTO_RX_LEX_RULE_INCLUDED
#define YOCTO_RX_LEX_RULE_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace lexical
		{
			
						
			
			//! pattern smart pointer
			class rule 
			{
			public:
				rule               *next;  //!< core::list binary layout
				rule               *prev;  //!< core::list binary layout
				pattern            *motif; //!< the one and only pattern
				const string        label; //!< for lexer database
				const size_t        flags;
				
				//! rule creationg
				/**
					\param p a valid pattern, handled in case of error
					\param l associated label
				 */
				static rule *create(  pattern *p, const string &l, const size_t f = 0);
				static void  destroy(  rule *r ) throw();			 
				
			private:
				rule( pattern *p, const string &l, const size_t f);
				~rule() throw();
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
		
			
		}
		
	}
	
}

#endif
