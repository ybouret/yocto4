#ifndef YOCTO_RX_SYN_S_NODE_INCLUDED
#define YOCTO_RX_SYN_S_NODE_INCLUDED 1

#include "yocto/rx/lexer.hpp"


namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			
			
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
				
				//! make a terminal node
				static s_node * create( lexeme *lx )
				{
					try
					{
						return new ( object::acquire1<s_node>() ) s_node( lx );
					}
					catch(...)
					{
						lexeme::destroy(lx);
						throw;
					}
				}
				
				//! make an internal node
				static s_node *create()
				{
					return new ( object::acquire1<s_node>() ) s_node( );
				}
				
				//! destroy with caching
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
				
				void append( s_node *node ) throw()
				{
					assert( internal == type );
					assert( NULL     == lx_  );
					ch_.store(node);
				}
				
				
			private:
				lexeme      *lx_; //!< lexeme     if terminal
				child_nodes  ch_; //!< child(ren) if internal
				
				explicit s_node( lexeme *lx ) throw() :
				type( terminal ),
				next(NULL),
				lx_( lx ),
				ch_()
				{
					
				}
				
				explicit s_node() throw() :
				type( internal ),
				next(NULL),
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
