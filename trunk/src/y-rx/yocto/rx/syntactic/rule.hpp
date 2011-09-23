#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	
	namespace regex
	{
		enum syntax_result
		{
			syntax_success, //!< found the right lexeme
			syntax_nothing, //!< found nothing: End of Source
			syntax_error,   //!< found unregistered token
			syntax_invalid, //!< found unexpected   token
		};
		
		namespace syntactic
		{
			
			
			
			class rule : public object
			{
			public:
				
				rule          *parent;
				const uint32_t type;
				const string   name;
				rule          *next;
				rule          *prev;
				
				virtual ~rule() throw();
				virtual  syntax_result match( lexer &, source &) = 0;
				
			protected:
				explicit rule( rule *p, uint32_t t, const string &id );
				
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
	}
}

#endif
