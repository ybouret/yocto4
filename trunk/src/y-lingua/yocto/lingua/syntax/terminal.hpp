#ifndef YOCTO_LINGUA_SYNTAX_TERMINAL_INCLUDED
#define YOCTO_LINGUA_SYNTAX_TERMINAL_INCLUDED 1

#include "yocto/lingua/syntax/rules.hpp"

namespace yocto
{
	namespace lingua
	{
        
		namespace syntax
		{
            
			//! match a lexeme with the same label
			class terminal : public rule
			{
			public:
				explicit terminal(const string   &id,
                                  node_property   ppty,
                                  const string   &eof_ref);
				virtual ~terminal() throw();
                
				Y_SYNTAX_RULE_API();
                
                const node_property semantic;
                
			private:
                const string &eof;
				YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
			};
            
		}
	}
}

#endif
