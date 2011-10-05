//#include "yocto/rx/syntax/c-node.hpp"
#include "yocto/rx/syntax/rule.hpp"
#include <iostream>
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			c_node * c_node:: create( lexeme *lx, rule &r )
			{
				try 
				{
					return new( object::acquire1<c_node>() ) c_node(lx,r);
				}
				catch (...) {
					lexeme::destroy(lx);
					throw;
				}
			}
			
			c_node *c_node:: create( rule &r )
			{
				return new( object::acquire1<c_node>() ) c_node(r);
			}
			
			void c_node:: destroy( c_node *node ) throw()
			{
				assert(node);
				//std::cerr << "destroy '"<< node->link.name << "'" << std::endl;
				
				switch( node->type )
				{
					case terminal:
						lexeme::destroy( node->data.lx );
						break;
						
					case internal:
						while( node->children().size > 0 ) destroy( node->children().pop_back() );
						break;
				}
				node->~c_node();
				object::release1<c_node>(node);
			}
			
			
			void c_node:: destroy( c_node *node, t_char::pool &tp) throw()
			{
				assert(node);
				switch( node->type )
				{
					case terminal:
						lexeme::destroy( node->data.lx, tp );
						break;
						
					case internal:
						while( node->children().size > 0 ) destroy( node->children().pop_back(), tp );
						break;
				}
				node->~c_node();
				object::release1<c_node>(node);
			}
			
			void c_node:: restore( c_node *node, lexer &lxr  ) throw()
			{
				assert(node);
				switch( node->type )
				{
					case terminal:
						lxr.unget( node->data.lx );
						break;
						
					case internal:
						while( node->children().size > 0 ) restore( node->children().pop_back(), lxr );
						break;
				}
				node->~c_node();
				object::release1<c_node>(node);
			}
			
			c_node:: c_node( lexeme *lx, rule &r ) throw() :
			type(terminal),
			next(NULL),
			prev(NULL),
			link(r),
			data()
			{
				data.lx = lx;
			}
			
			
			c_node:: c_node( rule &r ) throw() :
			type(internal),
			next(NULL),
			prev(NULL),
			link(r),
			data()
			{
				children().reset();
			}
			
			c_node:: ~c_node() throw() { assert( type == terminal || children().size <= 0 ); }
			
			c_node::child_nodes & c_node:: children() throw() { return *(child_nodes *) & data.wksp[0]; }
			const c_node::child_nodes & c_node:: children() const throw() { return *(const child_nodes *) & data.wksp[0]; }

			
			
			void c_node::append( c_node * &tree, c_node *node )
			{
				assert( NULL != node );
				if( tree == NULL ) 
				{
					//std::cerr << "*** setting tree to " << node->link.name << std::endl;
					tree = node;
				}
				else 
				{
					assert(internal==tree->type);
					//std::cerr << "*** appending '" << node->link.name << "' to '" << tree->link.name << "'" << std::endl;
					tree->children().push_back(node);
				}
				
			}
			
			void c_node:: viz( ios::ostream &os ) const
			{
				
				show_tag(os,this);
				os( " [label=\"%s\"", link.name.c_str() );
				if( type == terminal )
				{
					os( ",shape=oval];\n" );
					const string s = data.lx->to_string();
					show_tag(os,data.lx);
					os( "[label=\"%s\",shape=rect];\n", s.c_str() );
					show_tag(os,this); os( " -> " ); show_tag(os,data.lx); os("\n;");
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
