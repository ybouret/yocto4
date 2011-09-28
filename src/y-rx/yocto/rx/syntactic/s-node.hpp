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
				typedef core::list_of<s_node> child_nodes;
				
				
				enum content
				{
					terminal,
					internal,
					forsaken
				};
				
							
				//! no caching
				void release() throw()
				{
					switch( type )
					{
						case terminal:
							assert(0==ch_.size);
							lexeme::destroy( lx_ );
							break;
							
						case internal:
							assert(NULL==lx_);
							while( ch_.size )
							{
								s_node *n = ch_.pop_back();
								destroy(n);
							}
							break;
							
						case forsaken:
							break;
					}
					(content &)type = forsaken;
				}
				
				//! cache the used terminal token t_char
				void to( t_char::pool &tp ) throw()
				{
					switch( type )
					{
						case terminal:
							assert(0==ch_.size);
							lexeme::destroy( lx_, tp );
							break;
							
						case internal:
							assert(NULL==lx_);
							while( ch_.size )
							{
								s_node *n = ch_.pop_back();
								n->to(tp);
								destroy(n);
							}
							break;
							
						case forsaken:
							break;
					}
					(content &)type = forsaken;
				}
				
				
				const content type;
				s_node       *next;
				s_node       *prev;
				
				//! make a forsaken node
				static s_node *create() { return new ( object::acquire1<s_node>() ) s_node(); }
				static void    destroy( s_node *n ) throw() { assert(n); n->~s_node(); object::release1<s_node>(n); }
				
				//! make a terminal node
				static s_node *create( lexeme *lx )
				{
					try
					{
						s_node *n = create();
						n->lx_    = lx;
						(content &)(n->type) = terminal;
						return n;
					}
					catch(...)
					{
						lexeme::destroy( lx );
						throw;
					}
				}
				
				void append( s_node *child ) throw()
				{
					assert( terminal != type );
					assert( child != NULL );
					(content &)type = internal;
					ch_.push_back( child );
				}
				
				
			private:
				lexeme      *lx_; //!< lexem if terminal
				child_nodes  ch_; //!< child(ren) if internal
				
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(s_node);
				
				
				s_node() throw()  :
				type( forsaken ),
				next(NULL),
				prev(NULL),
				lx_(NULL),
				ch_()
				{
				}
				
				inline ~s_node() throw()
				{
					release();
				}
				
			};
		
	
						
			
		}
	}
}
#endif
