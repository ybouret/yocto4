#ifndef YOCTO_UTILS_INCLUDED
#define YOCTO_UTILS_INCLUDED 1


namespace yocto 
{
	
	template <typename T>
	inline T min_of( T a, T b ) 
	{
		return a < b ? a : b;
	}
	
	template <typename T>
	inline T max_of( T a, T b ) 
	{
		return b < a ? a : b;
	}

	extern const char hexachar[16];
	
}

#endif
