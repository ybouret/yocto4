#ifndef YOCTO_RX_SYN_JOKER_INCLUDED
#define YOCTO_RX_SYN_JOKER_INCLUDED 1

#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace syntactic
		{
		
			//! rule smart pointer
			class joker : public rule
			{
			public:
				virtual ~joker() throw();
				
				
			protected:
				rule *jk;
				explicit joker( uint32_t t, const string &n, rule *r );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
			};
			
		}
	}
}


#endif
