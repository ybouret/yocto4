#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/syntax/parse-node.hpp"

namespace yocto 
{
	namespace lang 
	{
		namespace syntax 
		{
                     
#define Y_SYNTAX_MATCH_ARGS \
lexer         &Lexer,       \
regex::source &Source,      \
parse_node *  &Tree
            
			class rule : public object 
			{
			public:
				const string label;
				rule        *next;
				rule        *prev;
				virtual ~rule() throw();
                
				virtual bool  match( Y_SYNTAX_MATCH_ARGS ) = 0;
                
                typedef rule *ptr;
			protected:
				explicit rule( const string &id );
                
				//! grow tree
                /**
                 if Tree != NULL, Tree must be NOT terminal
                 */
				void grow( parse_node * & Tree, parse_node * &Node ) throw();
                
                //! check the parent tree is valid
                void check( const parse_node *Tree);
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
            
            class rules : public core::list_of<rule>
            {
            public:
                explicit rules() throw();
                virtual ~rules() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rules);
            };
            
		}
	}
}


#endif
