#ifndef YOCTO_MATH_FIT_SAMPLE_INCLUDED
#define YOCTO_MATH_FIT_SAMPLE_INCLUDED 1


#include "yocto/sequence/array.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
			
			template <typename T>
			class sample
			{
			public:
				sample( const array<T> &ax, const array<T> &ay, array<T> &az ) throw();
				~sample() throw();
				
				const array<T> &x;
				const array<T> &y;
				array<T>       &z;
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(sample);
			};
		}
		
	}
}

#endif
