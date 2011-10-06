#include "yocto/rx/syntax/rule.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			void c_node:: viz( ios::ostream &os ) const
			{
				
				show_tag(os,this);
				os( " [label=\"%s\"", link.name.c_str() );
				if( type == terminal )
				{
					os( ",shape=oval];\n" );
					if( data.lx->size > 0 )
					{
						const string s = data.lx->to_string();
						show_tag(os,data.lx);
						os( "[label=\"%s\",shape=rect];\n", s.c_str() );
						show_tag(os,this); os( " ->  " ); show_tag(os,data.lx); os(" [ arrowhead=dot ]\n;");
					}
				}
				else {
					os( ",shape=invhouse];\n");
					for( const c_node *node = children().head; node; node=node->next )
					{
						node->viz(os);
						show_tag(os,this); os( " -> " ); show_tag(os,node); os("\n;");
					}
				}
				
			}
			
			void c_node:: graphviz( const string &graph_name, ios::ostream &os ) const
			{
				os( "digraph %s {\n", graph_name.c_str() );
				viz(os);
				os( "}\n");
			}
			
		}
		
	}
	
}
