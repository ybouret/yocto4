#include "yocto/ocl/real.hpp"


namespace yocto 
{
	namespace ocl 
	{
		const char As<float>:: Options[]  = "-DReal=float -DReal2=float2 -DReal4=float4 -DReal8=float8";
		const char As<double>::Options[] =  "-DYOCTO_KHR_FP64 -DReal=double -DReal2=double2 -DReal4=double4 -DReal8=double8";
	}

}

