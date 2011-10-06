#include "yocto/rx/syntax/terminal.hpp"
#include "yocto/rx/syntax/counting.hpp"
#include "yocto/rx/syntax/logical.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			void c_node:: format(  t_char::pool &tp ) throw()
			{
				switch( type )
				{
					case terminal:
						std::cerr << "-- format '" << link.name << "'" << std::endl;
						switch( link.type )
					{
						case certain::ID:
							data.lx->back_to(tp);
							break;
							
						default:
							break;
					}
						break;
						
						
					case internal:
					{
						child_nodes & __children = children();
						c_node      * node       = __children.head;
						while( node )
						{
							c_node * __next = node->next;
							switch( node->link.type )
							{
								case useless::ID:
									std::cerr << "-- remove '" << node->link.name << "'" << std::endl;
									c_node::destroy( __children.unlink(node), tp );
									break;
									
								case optional::   ID:
								case any_count::  ID:
								case one_or_more::ID:
								//case alternative::ID:
									std::cerr << "-- fusion '" << node->link.name << "'" << std::endl;
								{
									//-- take care of node
									node->format(tp);
									//-- fusion
									child_nodes left;
									while( node != __children.tail ) left.push_front( __children.pop_back() );
									__children.merge_back( __children.unlink(node)->children() );
									__children.merge_back( left );
									c_node::destroy( node );
								}
									break;
									
								default:
									node->format(tp);
									break;
							}
							
							node = __next;
						}
					}
						break;
				}
			}
		}
		
	}
	
}
