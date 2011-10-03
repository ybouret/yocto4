#ifndef YOCTO_RX_SYN_S_NODE_INCLUDED
#define YOCTO_RX_SYN_S_NODE_INCLUDED 1

#include "yocto/rx/lexer.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			
			class rule;
			
			//! syntactic node
			class s_node 
			{
			public:
				typedef core::list_of<s_node> child_nodes;
				
				
				enum content
				{
					terminal,
					internal
				};
				
				const content type; //!< internal/terminal
				s_node       *next; //!< for children
				s_node       *prev; //!< for children
				const rule   &link; //!< what to do
				
				//! make a terminal node
				static s_node * create( lexeme *lx, const rule &r  );
				
				//! make an internal node
				static s_node *create(const rule &r);
				
				//! destroy with t_char caching caching
				static void destroy( s_node *node, t_char::pool &tp ) throw();
				
				//! destroy without caching
				static void destroy(s_node *node ) throw();				
				
				//! destroy with lexemes caching
				static void restore( s_node *node, lexemes &cache ) throw();
				
				//! append a branch
				void append( s_node *node ) throw();
				
				//! register a graphivz subtree
				void viz( ios::ostream & os ) const;
				
			private:
				union  {
					lexeme  *lx;
					uint32_t wksp[ YOCTO_U32_FOR_ITEM(child_nodes) ];
				} data;
				
				explicit s_node( lexeme *lx, const rule &r ) throw();
				explicit s_node( const rule &r) throw();			
				~s_node() throw();
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(s_node);
				inline child_nodes &children()       throw() { return *(child_nodes *)(data.wksp); }
				inline child_nodes &children() const throw() { return *(child_nodes *)(data.wksp); }
				
			};
			
			
			
			
		}
	}
}
#endif
