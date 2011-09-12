#ifndef YOCTO_OCL_REAL_INCLUDED
#define YOCTO_OCL_REAL_INCLUDED 1


#include "yocto/ocl/_vec.hpp"

namespace yocto 
{
	namespace ocl 
	{
		
		template <typename T>
		struct As;
		
		template <>
		struct As<float>
		{
			typedef cl_float    Real;
			typedef float2_type Real2;
			static const char Options[];
		};
		
		template <>
		struct As<double>
		{
			typedef cl_double    Real;
			typedef double2_type Real2;
			static const char Options[];
		};
		
		
	}
}

#endif
