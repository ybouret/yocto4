#ifndef YOCTO_RX_SYN_S_NODE_INCLUDED
#define YOCTO_RX_SYN_S_NODE_INCLUDED 1

#include "yocto/rx/lexer.hpp"

//#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			
			class rule;
			
			class s_node 
			{
			public:
				typedef core::list_of<s_node> child_nodes;
				
				
				enum content
				{
					terminal,
					internal
				};
				
				
				
				const content type;
				s_node       *next;
				s_node       *prev;
				const rule   &link;
				
				//! make a terminal node
				static s_node * create( lexeme *lx, const rule &r  );
				
				//! make an internal node
				static s_node *create(const rule &r);
				
				//! destroy with t_char caching caching
				static void destroy( s_node *node, t_char::pool &tp ) throw()
				{
					assert( node );
					switch( node->type )
					{
						case terminal:
							lexeme::destroy(node->data.lx,tp);
							break;
							
						case internal:
							while( node->children().size ) destroy( node->children().pop_back(), tp );
							break;
					}
					node->~s_node();
					object::release1<s_node>(node);
				}
				
				//! destroy without caching
				static void destroy(s_node *node ) throw()
				{
					assert( node );
					switch( node->type )
					{
						case terminal:
							lexeme::destroy(node->data.lx);
							break;
							
						case internal:
							while( node->children().size ) destroy( node->children().pop_back() );
							break;
					}
					node->~s_node();
					object::release1<s_node>(node);
				}
				
				
				//! destroy with lexemes caching
				static void restore( s_node *node, lexemes &cache ) throw()
				{
					assert( node );
					switch( node->type )
					{
						case terminal:
							cache.push_front(node->data.lx);
							break;
							
						case internal:
							while( node->children().size ) restore( node->children().pop_back(), cache );
							break;
					}
					node->~s_node();
					object::release1<s_node>(node);
				}
				
				void append( s_node *node ) throw()
				{
					assert( internal == type );
					children().push_back(node);
				}
				
				void viz( ios::ostream & os ) const;
				
			private:
				//lexeme      *lx_; //!< lexeme     if terminal
				//child_nodes  ch_; //!< child(ren) if internal
				union  {
					lexeme  *lx;
					uint32_t wksp[ YOCTO_U32_FOR_ITEM(child_nodes) ];
				} data;
				
				explicit s_node( lexeme *lx, const rule &r ) throw() :
				type( terminal ),
				next(NULL),
				prev(NULL),
				link( r  ),
				data()
				{
					data.lx = lx;
				}
				
				explicit s_node( const rule &r) throw() :
				type( internal ),
				next(NULL),
				prev(NULL),
				link( r ),
				data()
				{
					memset( &data, 0, sizeof(data) );
				}
				
				~s_node() throw() { assert( terminal == type || 0 == children().size); }
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(s_node);
				inline child_nodes &children()       throw() { return *(child_nodes *)(data.wksp); }
				inline child_nodes &children() const throw() { return *(child_nodes *)(data.wksp); }

				
				
			};
			
			
			
			
		}
	}
}
#endif
