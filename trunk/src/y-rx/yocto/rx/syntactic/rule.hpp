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
				rule        *parent;
				const string name;
				virtual ~rule() throw();
				virtual  bool match( lexer &, source &) = 0;
				
			protected:
				explicit rule( rule *p, const string &id );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
			
			class terminal : public rule
			{
			public:
				virtual ~terminal() throw() {}
				
				explicit terminal( rule *p, const string &id ) : rule(p,id) {}
				
				virtual bool match( lexer &L, source &src)
				{
					lexeme *lx = L.lookahead( src );
					if( !lx )
						return false;
					else
					{
						if( lx->label == name )
						{
							
							return true;
						}
						else {
							return false;
						}

					}
				}
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
										
			};
			
		}
	}
}

#endif
