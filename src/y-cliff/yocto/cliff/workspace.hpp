#ifndef YOCTO_CLIFF_WORKSPACE_INCLUDED
#define YOCTO_CLIFF_WORKSPACE_INCLUDED 1

#include "yocto/cliff/types.hpp"

namespace yocto
{
	
	namespace cliff
	{
		template 
			<typename                  U,       // float/double for real coordinates
			 template <typename> class VERTEX   // vertex U
		>
		class workspace
		{
		public:
			typedef typename VERTEX<U>::type vtx;
			
		private:
			
		};
	}
	
}

#endif
