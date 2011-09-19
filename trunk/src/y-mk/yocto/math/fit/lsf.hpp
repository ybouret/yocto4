#ifndef YOCTO_MATH_FIT_LSF_INCLUDED
#define YOCTO_MATH_FIT_LSF_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
				
			template <typename T>
			class lsf
			{
			public:
				explicit lsf();
				virtual ~lsf() throw();
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(lsf);
			};
			
			
		}
		
	}
	
}

#endif
