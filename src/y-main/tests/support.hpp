#ifndef __SUPPORT_INCLUDED
#define __SUPPORT_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


template <typename T>
struct gen
{
	static inline T get() { return alea_of<T>(); }
};

template <>
struct gen<float> { static inline float get() { return alea<float>(); } };

template <>
struct gen<double> { static inline double get() { return alea<double>(); } };


template <>
struct gen<string> {
	static inline string get()
	{
		string ans;
		for( size_t i= 8 + alea_less_than<size_t>(20); i > 0; --i )
		{
			ans += char('A' + alea_less_than<int>(26) );
		}
		return ans;
	}	
};


#endif
