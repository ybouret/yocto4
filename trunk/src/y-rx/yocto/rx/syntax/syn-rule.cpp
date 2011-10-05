#include "yocto/rx/syntax/rule.hpp"
#include <iostream>

namespace yocto
{
	namespace regex
	{
		
		namespace syntax
		{
			
			rule:: ~rule() throw() {}
			
			rule:: rule( uint32_t t, const string &n, grammar &g) :
			type(t),
			name(n),
			G(g),
			nref_(0),
			ppty(0)
			{
			}
			
			
			void   rule:: withhold() throw() 
			{
				++nref_;
			}
			
			bool   rule:: liberate() throw() 
			{ 
				assert( nref_ > 0 );
				return --nref_ <= 0; 
			}
			
			size_t rule:: refcount() const throw() { return nref_; }
			
		}

	}

}
