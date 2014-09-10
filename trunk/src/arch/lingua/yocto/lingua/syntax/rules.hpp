#ifndef YOCTO_LINGUA_SYNTAX_RULES_INCLUDED
#define YOCTO_LINGUA_SYNTAX_RULES_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
     
#define Y_SYNTAX_MATCH_TYPE bool
#define Y_SYNTAX_MATCH_ARGS lexer &Lexer, source &Source, xnode * &Tree
#define Y_SYNTAX_RULE_API() \
virtual Y_SYNTAX_MATCH_TYPE match( Y_SYNTAX_MATCH_ARGS )
            
            class rule : public object
			{
			public:
				const string label;
				rule        *next;
				rule        *prev;
				virtual ~rule() throw();
                
				virtual Y_SYNTAX_MATCH_TYPE match( Y_SYNTAX_MATCH_ARGS ) = 0;
                
                typedef rule *ptr;
                
			protected:
				explicit rule( const string &id );
                
                void check( const xnode *tree );
                void grow(xnode * & Tree, xnode * &Node) throw();
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
            
            class r_list : public core::list_of<rule>
            {
            public:
                explicit r_list() throw();
                virtual ~r_list() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(r_list);
            };
            
            
        }
        
    }
    
}

#endif

