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
				
				static c_node *create( lexeme *lx, rule &r );				
				static c_node *create( rule &r );				
				
				static void destroy( c_node *node ) throw();				
				static void destroy( c_node *node, t_char::pool &tp) throw();
				static void restore( c_node *node, lexer &lxr      ) throw();
				
				static void append( c_node * &tree, c_node *node )
				{
					assert( NULL != node );
					if( tree == NULL ) 
					{
						tree = node;
					}
					else 
					{
						assert(internal==tree->type);
						tree->children().push_back(node);
					}

				}
				
			private:
				explicit c_node( lexeme *lx, rule &r ) throw();				
				explicit c_node( rule &r ) throw() ;								
				~c_node() throw();
				
				union 
				{
					lexeme  *lx;
					uint32_t wksp[ YOCTO_U32_FOR_ITEM(child_nodes) ];
				} data;
				
				child_nodes & children() throw();
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(c_node);
			};
			
		}
		
	}
	
}

#endif
