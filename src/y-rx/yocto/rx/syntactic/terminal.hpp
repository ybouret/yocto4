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
						
				static terminal * create( const string &n);
				static terminal * create( const char   *n);
				
			private:
				explicit terminal(const string &n );	
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
				virtual syntax::result match( lexer &lxr, source &src, lexemes &stk );

			};
			
			
		}
		
		typedef syntactic::terminal s_terminal;
		
	}
	
	
}

#endif
