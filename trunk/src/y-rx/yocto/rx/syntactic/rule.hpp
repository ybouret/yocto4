#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/syntactic/s-node.hpp"
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
				unexpected    = 0x02  //!< a valid lexeme, unexpected here
			};
			static const char *get_info( syntax::result ) throw();
		};
		
		namespace syntactic
		{
			typedef yocto::imported::exception exception;
			
			
#define YOCTO_RX_SYNTAX_RULE_MATCH_ARGS lexer &lxr, source &src, lexemes &stk
			
			class rule : public object
			{
			public:
				
				const uint32_t type;
				const string   name;
				rule          *next;
				rule          *prev;
				const rule    *parent;
				
				virtual ~rule() throw();
				
				virtual syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS ) = 0;
				
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
