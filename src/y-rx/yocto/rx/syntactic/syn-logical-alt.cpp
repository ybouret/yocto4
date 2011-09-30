#include "yocto/rx/syntactic/logical.hpp"
#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			
			//==================================================================
			//
			//==================================================================
			
			OR:: OR( const string &n ) :
			logical( ID, n )
			{
			}
			
			OR:: OR( const OR &other ) :
			logical( other )
			{
				
			}
			
			OR * OR:: create( const string &id )
			{
				return new OR(id);
			}
			
			OR * OR:: create( const char *txt )
			{
				const string id( txt );
				return new OR(id);
			}
			
			rule * OR:: clone() const 
			{
				return new OR( *this );
			}
			
			OR:: ~OR() throw()
			{
				
			}
			
			syntax::result OR:: match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				std::cerr << "?<OR=" << name << ">" << std::endl;
				
				check(tree);
				s_node *local_tree = s_node::create( *this );
				syntax::result res = syntax::success;
				for( rule *r = operands.head; r; r=r->next )
				{
					res = r->match(lxr, src, local_tree);
					if( res == syntax::success )
					{
						std::cerr << "</OR> = '" << r->name << "'" << std::endl;
						if( tree )
						{
							assert( s_node::internal == tree->type );
							tree->append( local_tree );
						}
						else 
						{
							tree = local_tree;
						}
						
						return syntax::success;
					}
				}
				
				return res;
			}
			
		}
		
	}
	
}
