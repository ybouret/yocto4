#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		const char *syntax::get_info( syntax::result r) throw()
		{
			switch (r) {
				case syntax::success:
					return "syntax::success";
					
				case syntax::nothing:
					return "syntax::nothing";
					
				case syntax::unexpected:
					return "syntax::unexpected";
					
				default:
					break;
			}
			return "syntax::unknown";
		}
		
		namespace syntactic
		{
			
			rule:: ~rule() throw() {}
			
			rule:: rule( uint32_t t, const string &id ) : 
			type(t),
			name( id ),
			next(NULL),
			prev(NULL),
			parent(NULL)
			{}
			
			rules:: rules() throw() {}
			
			static inline 
			void __delete_rule( rule *r ) throw() { assert(NULL!=r); delete r; }
			
			rules:: ~rules() throw() { delete_with( __delete_rule ); }
			
			
			void rule:: unwind( exception &e ) const throw()
			{
				const rule *p = this;
				while( p )
				{
					e.cat( "=>%s", p->name.c_str() );
					p = p->parent;
				}
			}
			
			
		}
		
	}
	
}
