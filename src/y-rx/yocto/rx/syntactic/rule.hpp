#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		struct syntax
		{
			enum result
			{
				success       = 0x00, //!< normal return
				nothing       = 0x01, //!< end of source
				unexpected    = 0x02 //!< a valid lexeme, unexpected here
			};
		};
		
		namespace syntactic
		{
			typedef yocto::imported::exception                             exception;
			
			class rule : public object
			{
			public:
				
				const uint32_t type;
				const string   name;
				rule          *next;
				rule          *prev;
				const rule    *parent;
				
				virtual ~rule() throw();
				
				virtual syntax::result match(lexer        &lxr, 
											 source       &src,
											 lexemes      &stk) = 0;
				
				void unwind( exception & ) const throw();
				
				
			protected:
				explicit rule( uint32_t t, const string &id );
				
			private:
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
		
		typedef syntactic::rule s_rule;
	}
}

#endif
