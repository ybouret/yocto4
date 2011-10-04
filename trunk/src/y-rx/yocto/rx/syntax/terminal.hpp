#ifndef YOCTO_RX_TERMINAL_INCLUDED
#define YOCTO_RX_TERMINAL_INCLUDED 1

#include "yocto/rx/syntax/rule.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			class terminal : public rule
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('T','E','R','M');
				virtual ~terminal() throw();
				explicit terminal( const string &n, grammar &g);

			private:
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
			};
			
		}
				
	}
	
}

#endif
