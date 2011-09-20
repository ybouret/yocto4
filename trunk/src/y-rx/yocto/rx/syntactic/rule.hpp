#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/lexer.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace syntactic
		{
			class rule : public object
			{
			public:
				virtual ~rule() throw();
				virtual  bool match( lexer & ) = 0;
				
			protected:
				explicit rule() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
		}
	}
}

#endif
