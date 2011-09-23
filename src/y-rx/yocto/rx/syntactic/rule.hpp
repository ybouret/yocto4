#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	
	namespace regex
	{
		enum syntax_result
		{
			syntax_success, //!< found the right lexeme
			syntax_nothing, //!< found nothing: End of Source
			syntax_error,   //!< found unregistered token
			syntax_invalid, //!< found unexpected   token
		};
		
		namespace syntactic
		{
			
			
			
			class rule : public object
			{
			public:
				
				rule        *parent;
				const string name;
				rule        *next;
				rule        *prev;
				
				virtual ~rule() throw();
				virtual  syntax_result match( lexer &, source &) = 0;
				
			protected:
				explicit rule( rule *p, const string &id );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
#if 1
			class terminal : public rule
			{
			public:
				virtual ~terminal() throw() {}
				
				explicit terminal( rule *p, const string &id ) : rule(p,id) {}
				
				virtual syntax_result match( lexer &L, source &src)
				{
					lexeme *lx = L.lookahead( src );
					if( !lx )
						return src.is_active() ? syntax_error : syntax_error;
					else
					{
						if( lx->label == name )
						{
							
							return syntax_success;
						}
						else {
							return syntax_invalid;
						}
						
					}
				}
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
				
			};
#endif
			
		}
	}
}

#endif
