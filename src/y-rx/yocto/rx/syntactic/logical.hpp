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

			class logical : public rule
			{
			public:
				rules operands;
				virtual ~logical() throw();
				
			protected:
				explicit logical( uint32_t t, const string &id );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(logical);
			};
			
			class AND : public logical
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('L','G','&','&');
				explicit AND( const string &id );
				virtual ~AND() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(AND);
				virtual  syntax::result analyze( lexer &lxr, source &src, lexemes &stk);
			};
		}

	}

}
#endif
