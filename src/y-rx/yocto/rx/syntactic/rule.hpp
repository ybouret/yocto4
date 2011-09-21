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
				const string name;
				virtual ~rule() throw();
				virtual  bool match( lexer & ) = 0;
				
			protected:
				explicit rule( const string &id );
				rule( const rule & );
				
			private:
				YOCTO_DISABLE_ASSIGN(rule);
			};
			
			
		}
	}
}

#endif
