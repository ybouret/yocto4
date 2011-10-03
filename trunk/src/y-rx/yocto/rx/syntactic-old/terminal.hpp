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
				
				const bool keeping;
				
				virtual ~terminal() throw();
						
				static terminal * create( const string &n, bool keep = true);
				static terminal * create( const char   *n, bool keep = true );
				
				virtual rule *clone() const;
				
			protected:
				explicit terminal(const string &n, bool k);	

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
				virtual syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS );
			};
			
			class symbol : public terminal
			{
			public:
				virtual ~symbol() throw() {}
				
				static symbol * create( const string &n ) { return new symbol(n); }
				static symbol * create( const char   *n ) { const string s(n); return new symbol(n); }
				
			private:
				explicit symbol( const string &n ) : terminal(n,false) {}
				YOCTO_DISABLE_COPY_AND_ASSIGN(symbol);
			};
			
		}
		
		typedef syntactic::terminal s_terminal;
		typedef syntactic::symbol   s_symbol;
		
	}
	
	
}

#endif
