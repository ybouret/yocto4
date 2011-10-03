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
			
			AND:: AND( const string &n ) :
			logical( ID, n )
			{
			}
			
			AND:: AND( const AND &other ) :
			logical( *this )
			{
				
			}
			
			AND * AND:: create( const string &id )
			{
				return new AND(id);
			}
			
			AND * AND:: create( const char *txt )
			{
				const string id( txt );
				return new AND(id);
			}
			
			
			rule * AND::clone() const
			{
				return new AND( *this );
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			syntax::result AND:: match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				space(std::cerr) << "?AND='" << name << "'" << std::endl;
				
				check(tree);
				s_node *local_tree = s_node::create(*this);
				try
				{
					//-- must match all rules
					for( rule *r = operands.head; r; r=r->next )
					{
						const syntax::result res = r->match(lxr, src, local_tree);
						if( res != syntax::success )
						{
							s_node::restore( local_tree, lxr.cache );
							return res;
						}
					}
					
					//-- all done !
					if( tree )
					{
						assert( s_node::internal == tree->type );
						tree->append( local_tree );
					}
					else 
					{
						tree = local_tree;
					}
					std::cerr << "/AND='" << name << "'" << std::endl;
					return syntax::success;
				}
				catch(...)
				{
					s_node::restore( local_tree, lxr.cache );
					throw;
				}
			}
			
		}
		
	}
	
}
