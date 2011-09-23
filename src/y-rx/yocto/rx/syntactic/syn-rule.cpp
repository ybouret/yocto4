#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace syntactic
		{

			rule:: ~rule() throw() {}
			
			rule:: rule( rule *p, uint32_t t, const string &id ) : 
			parent(p),
			type(t),
			name( id ),
			next(NULL),
			prev(NULL)
			{}
			
			rules:: rules() throw() {}
			
			static inline 
			void __delete_rule( rule *r ) throw() { assert(NULL!=r); delete r; }
			
			rules:: ~rules() throw() { delete_with( __delete_rule ); }
			
		}

	}

}
