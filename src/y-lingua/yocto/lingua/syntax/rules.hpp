#ifndef YOCTO_LINGUA_SYNTAX_RULES_INCLUDED
#define YOCTO_LINGUA_SYNTAX_RULES_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
            
#define Y_SYNTAX_MATCH_ARGS lexer &Lexer, source &Source, xnode * &Tree
            
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
				void grow( xnode * & Tree, xnode * &Node ) throw();
                
                //! check the parent tree is valid
                void check( const xnode *Tree);
                
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

