#if 0
#include "yocto/rx/syntax/terminal.hpp"
#include "yocto/rx/syntax/counting.hpp"
#include "yocto/rx/syntax/logical.hpp"
#endif

#include "yocto/rx/grammar.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		
		
		namespace syntax
		{
			
			void c_node:: format(  grammar &G, t_char::pool &tp ) throw()
			{
				switch( type )
				{
					case terminal:
						std::cerr << "-- format '" << link.name << "'" << std::endl;
						if( link.ppty & node_certain )
							data.lx->back_to(tp);
						else 
						{
							G.apply( link.name, *(data.lx) );
						}

						break;
						
						
					case internal:
					{
						child_nodes & __children = children();
						c_node      * node       = __children.head;
						while( node )
						{
							c_node * __next = node->next;
							if( node->link.ppty & node_useless )
							{
								std::cerr << "-- remove '" << node->link.name << "'" << std::endl;
								c_node::destroy( __children.unlink(node), tp );
								node = __next;
								continue;
							}
							
							if( node->link.ppty & node_fusion )
							{
								std::cerr << "-- fusion '" << node->link.name << "'" << std::endl;
								//-- take care of node
								node->format(G,tp);
								//-- fusion
								child_nodes left;
								while( node != __children.tail ) left.push_front( __children.pop_back() );
								__children.merge_back( __children.unlink(node)->children() );
								__children.merge_back( left );
								c_node::destroy( node );
								node = __next;
								continue;
							}
							
							node->format(G,tp);
							node = __next;
						}
					}
						break;
				}
			}
		}
		
	}
	
}
