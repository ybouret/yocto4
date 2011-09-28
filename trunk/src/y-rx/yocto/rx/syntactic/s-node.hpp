#ifndef YOCTO_RX_SYN_S_NODE_INCLUDED
#define YOCTO_RX_SYN_S_NODE_INCLUDED 1

#include "yocto/rx/lexer.hpp"
#include "yocto/rx/source.hpp"


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
				
							
				
				void release( lexer &lxr, source &src ) throw()
				{
					switch( type )
					{
						case terminal:
							assert(0==ch_.size);
							lxr.drop( lx_, src  );
							lx_ = NULL;
							break;
							
						case internal:
							assert(NULL==lx_);
							while( ch_.size )
							{
								s_node *n = ch_.pop_back();
								n->release(lxr,src);
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
				
				static s_node *create() { return new ( object::acquire1<s_node>() ) s_node(); }
				static void    destroy( s_node *n ) throw() { assert(n); n->~s_node(); object::release1<s_node>(n); }
				
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
					assert(0==ch_.size);
				}
				
			};
			
		}
		
	}
	
}
#endif
