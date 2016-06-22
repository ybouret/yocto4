#ifndef __SUPPORT_INCLUDED
#define __SUPPORT_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/complex.hpp"

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
		for( size_t i= 8 + alea_leq(10); i > 0; --i )
		{
			ans += char('A' + alea_lt(26) );
		}
		return ans;
	}	
};

template <>
struct gen< math::complex<float> >
{
	static inline math::complex<float> get() { return math::complex<float>( alea<float>(), alea<float>() ); }
};

template <>
struct gen< math::complex<double> >
{
	static inline math::complex<double> get() { return math::complex<double>( alea<double>(), alea<double>() ); }
};

#endif
