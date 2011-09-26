#ifndef YOCTO_RX_SYN_TERMINAL_INCLUDED
#define YOCTO_RX_SYN_TERMINAL_INCLUDED 1

#include "yocto/rx/syntactic/rule.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			class terminal : public rule
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('T','E','R','M');
				virtual ~terminal() throw();
				explicit terminal(const string &n );			

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
				virtual syntax::result analyze( lexer &lxr, source &src, lexemes &stk);

			};
			
			
		}
		
	}
	
	
}

#endif
