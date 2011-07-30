#ifndef YOCTO_SPADE_COMPOSITION_INCLUDED
#define YOCTO_SPADE_COMPOSITION_INCLUDED 1

#include "yocto/spade/types.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		class composition	
		{
		public:
			explicit composition( unit_t lo, unit_t hi ) throw();
			virtual ~composition() throw();
			composition( const composition &other ) throw();
			
			const unit_t index_min;
			const unit_t index_max;
			const size_t components;
			
		private:
			YOCTO_DISABLE_ASSIGN(composition);
		};
		
	}
	
}

#endif
