#include "yocto/rx/syntactic/rule.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			
			s_node * s_node:: create( lexeme *lx, const rule &r  )
			{
				try
				{
					return new ( object::acquire1<s_node>() ) s_node( lx, r );
				}
				catch(...)
				{
					lexeme::destroy(lx);
					throw;
				}
			}
			
			s_node * s_node:: create(const rule &r)
			{
				return new ( object::acquire1<s_node>() ) s_node(r);
			}
			
			void s_node:: destroy( s_node *node, t_char::pool &tp ) throw()
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
			
			void s_node:: destroy(s_node *node ) throw()
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
			
			
			void s_node:: restore( s_node *node, lexemes &cache ) throw()
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
			
			void s_node:: append( s_node *node ) throw()
			{
				assert( internal == type );
				children().push_back(node);
			}
			
			
			s_node:: s_node( lexeme *lx, const rule &r ) throw() :
			type( terminal ),
			next(NULL),
			prev(NULL),
			link( r  ),
			data()
			{
				data.lx = lx;
			}
			
			s_node:: s_node( const rule &r) throw() :
			type( internal ),
			next(NULL),
			prev(NULL),
			link( r ),
			data()
			{
				memset( &data, 0, sizeof(data) );
			}
			
			s_node:: ~s_node() throw() { assert( terminal == type || 0 == children().size); }
			
			
			
			void s_node:: viz( ios::ostream & os ) const
			{
				show_tag( os, this );
				const char *shape = type == internal ? "oval" : "box";
				os(" [ label=\"%s\",shape=%s];\n", link.name.c_str(),shape);
				if( type == internal )
				{
					for( const s_node *node = children().head; node; node=node->next)
					{
						node->viz(os);
						show_tag(os,this); os.append( " -> " ); show_tag(os,node); os.append( ";\n");
					}
				}
				else
				{
					const lexeme *lx = data.lx;
					if( lx->size > 0 )
					{
						show_tag( os, lx ); 
						const string s = lx->to_string();
						os(" [ label=\"%s\",shape=house ];", s.c_str() );
						os.append("\n");
						show_tag(os,this); os.append( " -> " ); show_tag(os,lx); os.append( ";\n");
					}
				}
			}
			
		}
		
	}
	
}
