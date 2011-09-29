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
			}
			
		}
		
	}
	
}
