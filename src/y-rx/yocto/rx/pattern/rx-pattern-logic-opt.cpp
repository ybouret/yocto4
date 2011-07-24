#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{

	namespace regex
	{

		namespace logical
		{

			void AND:: optimize() throw()
			{
				p_list tmp;
				while( operands.size )
				{
					pattern *p = operands.pop_front();
					switch( p->type )
					{
					case AND::id:
						assert(p->data);
						static_cast<AND *>(p->data)->optimize();
						tmp.merge_back( static_cast<AND *>(p->data)->operands );
						delete p;
						break;

					case OR::id:
						assert(p->data);
						static_cast<OR *>(p->data)->optimize();
						tmp.push_back( p );
						break;

					default:
						tmp.push_back( p );
					}
				}
				mswap(tmp,operands);
			}


			void OR:: optimize() throw()
			{
				// TODO: no multiple single...
				p_list tmp;
				while( operands.size )
				{
					pattern *p = operands.pop_front();
					switch( p->type )
					{
					case AND::id:
						assert(p->data);
						static_cast<AND *>(p->data)->optimize();
						tmp.push_back( p );
						break;

					case OR::id:
						assert(p->data);
						static_cast<OR *>(p->data)->optimize();
						tmp.merge_back( static_cast<OR *>(p->data)->operands );
						delete p;
						break;

					default:
						tmp.push_back( p );
					}
				}
				mswap(tmp,operands);
			}

		}

	}

}
