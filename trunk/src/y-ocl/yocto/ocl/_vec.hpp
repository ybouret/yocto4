#ifndef YOCTO_OPENCL_VEC_INCLUDED
#define YOCTO_OPENCL_VEC_INCLUDED 1

#include "yocto/ocl/types.hpp"

namespace yocto
{
	
	namespace ocl 
	{
		
		
		namespace Core
		{
			template <typename T> 
			union Vec2 
			{
				
				T s[2];
				struct { T x,y; };
				struct { T s0,s1; };
				
			};
			
			template <typename T> 
			union Vec4
			{
				
				T s[4];
				struct { T x,y,z,w; };
				struct { T s0,s1,s2,s3; };
				
			};
			
			template <typename T> 
			union Vec8
			{
				
				T s[8];
				struct { T s0,s1,s2,s3,s4,s5,s6,s7; };
				
			};
		}
	
#define YOCTO_OPENCL_DECL_(N,TYPE) typedef Core::Vec##N<cl_##TYPE> TYPE##N##_type

#define YOCTO_OPENCL_DECL_VEC(N) \
YOCTO_OPENCL_DECL_(N,int);\
YOCTO_OPENCL_DECL_(N,uint);\
YOCTO_OPENCL_DECL_(N,float);\
YOCTO_OPENCL_DECL_(N,double)
		
		YOCTO_OPENCL_DECL_VEC(2);
		YOCTO_OPENCL_DECL_VEC(4);
		YOCTO_OPENCL_DECL_VEC(8);
		
			
	}
	
	
	
}

#endif
