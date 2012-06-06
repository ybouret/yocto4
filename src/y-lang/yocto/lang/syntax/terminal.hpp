#ifndef YOCTO_LANG_SYNTAX_TERMINAL_INCLUDED
#define YOCTO_LANG_SYNTAX_TERMINAL_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
	namespace lang
	{

		namespace syntax
		{

			//! match a lexeme with the same label
			class terminal : public rule
			{
			public:
				explicit terminal( const string &id, bool meaningfull );
				virtual ~terminal() throw();

				virtual bool  match( Y_SYNTAX_MATCH_ARGS );
                
                const bool semantic;
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
			};

		}
	}
}

#endif
