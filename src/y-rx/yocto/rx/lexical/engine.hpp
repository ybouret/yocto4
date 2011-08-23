#ifndef YOCTO_RX_LEXICAL_ENGINE_INCLUDED
#define YOCTO_RX_LEXICAL_ENGINE_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
	namespace regex
	{
	
		
		typedef uint32_t label_type;
		
		namespace lexical
		{
		
			
			class engine : public object
			{
			public:
				engine( label_type l ) throw();
				engine( const engine &other );
				virtual ~engine() throw();
				
				
				const label_type label;
				
				inline const label_type & key() const throw() { return label; }
				const char *name() const throw();
				
				//! add a pattern => rule
				void operator()( pattern *p, rule *r ) throw();
				
				//! create a pattern => action
				void make( pattern *p, const action &a );
				template <typename HOST>
				inline void make( pattern *p, HOST *host, void (HOST::*method)( const token & ) )
				{
					assert(method);
					a
					auto_ptr<pattern> q(p);
					const action      a(host,method);
					
				}
				
			private:
				p_list        accepters;
				r_list        callbacks;
				char name_[ 8 ];
				YOCTO_DISABLE_ASSIGN(engine);
			};
			
			typedef set<label_type,engine> engines;

			
		}
		
	}
}

#endif
