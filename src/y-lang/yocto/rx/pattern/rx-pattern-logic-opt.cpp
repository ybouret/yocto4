#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace logical
		{
			
			void AND:: optimize()
			{
				p_list tmp;
				while( operands.size )
				{
					pattern *p = operands.pop_front();
					p->optimize();
					switch( p->type )
					{
						case AND::id:
							assert(p->data);
							tmp.merge_back( static_cast<AND *>(p->data)->operands );
							delete p;
							break;
							
						default:
							tmp.push_back( p );
					}
				}
				bswap(tmp,operands);
			}
			
			
			void OR:: optimize() 
			{
				
				//-- pass 1 : recursive opt
				p_list  tmp;
				while( operands.size )
				{
					pattern *p = operands.pop_front();
					p->optimize();
					switch( p->type )
					{
						case OR::id:
							assert(p->data);
							tmp.merge_back( static_cast<OR *>(p->data)->operands );
							delete p;
							break;
							
							
						default:
							tmp.push_back( p );
					}
				}
				
				//-- pass 2: no multiple single
				symbols sdb;
				while( tmp.size )
				{
					auto_ptr<pattern> p( tmp.pop_front() );
					if( p->type == basic::single::id )
					{
						assert( p->data );
						const char C = *static_cast<char *>(p->data);
						if( ! sdb.insert( C ) )
						{
							//multiple single
							continue;
						}
					}
					operands.push_back(p.yield());
				}
				
			}
			
		}
		
	}
	
}
