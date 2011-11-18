//! \file

#ifndef YOCTO_RX_SYN_C_NODE_INCLUDED
#define YOCTO_RX_SYN_C_NODE_INCLUDED 1

#include "yocto/rx/lexer.hpp"

namespace yocto
{
	
	namespace regex
	{
	
		
		namespace syntax
		{
			
			
			
			class rule;
			
			//! concrete syntax node
			class c_node
			{
			public:
				enum type_t
				{
					internal,
					terminal
				};
				typedef core::list_of<c_node> child_nodes;
				
				const type_t  type;
				c_node       *next;
				c_node       *prev;
				const rule   &link;
				
				static c_node *create( lexeme *lx, rule &r ); //!< create terminal
				static c_node *create( rule &r );		      //!< create internal
				
				static void destroy( c_node *node ) throw();                  //!< full destruction
				static void restore( c_node *node, lexer &lxr      ) throw(); //!< unget the node
				
				static void append( c_node * &tree, c_node *node ); //!< grow the tree
					
				void graphviz( const string &graph_name, ios::ostream &os ) const;
				void graphviz( const char   *graph_name, ios::ostream &os ) const { const string s(graph_name); graphviz(s,os); }
				
				void compile() throw();
				
			private:
				explicit c_node( lexeme *lx, rule &r ) throw();				
				explicit c_node( rule &r ) throw() ;								
				~c_node() throw();
				
				union 
				{
					lexeme  *lx;
					uint32_t wksp[ YOCTO_U32_FOR_ITEM(child_nodes) ];
				} data;
				
				child_nodes       & children() throw();
				const child_nodes & children() const throw();
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(c_node);
				void viz( ios::ostream & ) const;
			};
			
		}
		
	}
	
}

#endif
