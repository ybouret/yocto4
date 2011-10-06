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
				//static const uint32_t ID = YOCTO_FOURCC('T','E','R','M');
				virtual ~terminal() throw();
				
			protected:
				explicit terminal( uint32_t type, const string &n, grammar &g );

			private:
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
			};
			
			
			//! undefined terminal
			class variant : public terminal
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('T','V','A','R');
				explicit variant( const string &n, grammar &g );
				virtual ~variant() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(variant);
			};
			
			//! only one possible terminal
			class certain : public terminal
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('T','C','E','R');
				explicit certain( const string &n, grammar &g );
				virtual ~certain() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(certain);
			};
			
			//! syntax only terminal, to drop
			class useless : public terminal
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('T','N','I','L');
				explicit useless( const string &n, grammar &g );
				virtual ~useless() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(useless);
				
			};
			
			
			
		}
				
	}
	
}

#endif
