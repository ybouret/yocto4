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
				
				static terminal *create( const string & );
			private:
				explicit terminal( const string &n );
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
			};
			
		}
		inline syntax::terminal *new_terminal( const string &n  ) { return syntax::terminal::create( n ); }
		inline syntax::terminal *new_terminal( const char   *txt) { const string n(txt); return new_terminal(n); }
		
	}
	
}

#endif
