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
				typedef core::pool_of<s_node> child_nodes;
				
				
				enum content
				{
					terminal,
					internal
				};
				
				
				
				const content type;
				s_node       *next;
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
							lexeme::destroy(node->lx_,tp);
							break;
							
						case internal:
							node->ch_.delete_with<t_char::pool&>( destroy, tp );
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
							lexeme::destroy(node->lx_);
							break;
							
						case internal:
							node->ch_.delete_with( destroy );
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
							cache.push_front(node->lx_);
							break;
							
						case internal:
							node->ch_.delete_with<lexemes&>( restore, cache );
							break;
					}
					node->~s_node();
					object::release1<s_node>(node);
				}
				
				void append( s_node *node ) throw()
				{
					assert( internal == type );
					assert( NULL     == lx_  );
					ch_.store(node);
				}
				
				void viz( ios::ostream & os ) const;
				
			private:
				lexeme      *lx_; //!< lexeme     if terminal
				child_nodes  ch_; //!< child(ren) if internal
				
				explicit s_node( lexeme *lx, const rule &r ) throw() :
				type( terminal ),
				next(NULL),
				link( r  ),
				lx_( lx ),
				ch_()
				{
					
				}
				
				explicit s_node( const rule &r) throw() :
				type( internal ),
				next(NULL),
				link( r ),
				lx_(NULL),
				ch_()
				{
					
				}
				
				~s_node() throw() { assert(0 == ch_.size); }
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(s_node);
				
				
				
			};
			
			
			
			
		}
	}
}
#endif
