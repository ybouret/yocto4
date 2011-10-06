#include "yocto/rx/syntax/logical.hpp"
#include "yocto/rx/grammar.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			////////////////////////////////////////////////////////////////////
			logical:: ~logical() throw()
			{
			}
			
			logical:: logical( uint32_t t, const string &n, int p, grammar &g ) :
			rule(t,n,p,g),
			operands(2,as_capacity)	
			{
			}
			
			logical & logical:: operator<<( const string &p )
			{
				operands.push_back( G[p] );
				return *this;
			}
			
			logical & logical:: operator<<( const char *p )
			{
				operands.push_back( G[p] );
				return *this;
			}
			
			
			////////////////////////////////////////////////////////////////////
			aggregate:: aggregate( const string &n, int p, grammar &g ) :
			logical(ID,n,p,g)
			{
				
			}
			
			aggregate:: ~aggregate() throw()
			{
			}
			
			
			
			syntax_result aggregate:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				c_node *local_tree = c_node::create( *this );
				try
				{
					std::cerr << "?match AGGR " << name << " [";
					for( size_t i=1; i <= operands.size(); ++i )
					{
						std::cerr << " " << operands[i]->name;
					}
					std::cerr << " ]" << std::endl;
					for( size_t i=1; i <= operands.size(); ++i )
					{
						const syntax_result res = operands[i]->match( lxr, src, local_tree );
						if( res != syntax_success )
						{
							std::cerr << "-match AGGR " << name  << std::endl;
							c_node::restore( local_tree, lxr );
							return res;
						}
					}
					std::cerr << "+match AGGR " << name  << std::endl;
					c_node::append( tree, local_tree );
					return syntax_success;
				}
				catch(...)
				{
					c_node::destroy( local_tree );
					throw;
				}
			}
			
			////////////////////////////////////////////////////////////////////
			alternative:: alternative( const string &n, grammar &g ) :
			logical(ID,n,0,g)
			{
				
			}
			
			alternative:: ~alternative() throw()
			{
			}
			
			
			
			syntax_result alternative:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				
				std::cerr << "?match ALTR " << name << " [";
				for( size_t i=1; i <= operands.size(); ++i )
				{
					std::cerr << " " << operands[i]->name;
				}
				std::cerr << " ]" << std::endl;
				syntax_result res = syntax_success;
				for( size_t i=1; i <= operands.size(); ++i )
				{
					c_node *node = NULL;
					res = operands[i]->match( lxr, src, node );
					if( res == syntax_success )
					{
						assert( node != NULL );
						std::cerr << "+match ALTR " << name  << std::endl;
						c_node::append( tree, node );
						return syntax_success;
					}
#if !defined(NDEBUG)
					else {
						assert(node==NULL);
					}
#endif
				}
				std::cerr << "-match ALTR " << name  << std::endl;
				return res;
			}
			
		}
		
		
	}
	
}
