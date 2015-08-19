#ifndef YOCTO_MATH_ZTYPE_INCLUDED
#define YOCTO_MATH_ZTYPE_INCLUDED 1

#include "yocto/math/complex.hpp"

#include <cfloat>

namespace yocto 
{
	namespace math
	{
		//======================================================================
		//
		// simple precision types
		//
		//======================================================================
		
		
#	if (YOCTO_ZTYPE == 's') || (YOCTO_ZTYPE == 'c')
		typedef float           real_t;
		typedef complex<real_t> cplx_t;
		
#		if YOCTO_ZTYPE == 's'
		typedef real_t z_type;
#		endif
		
#		if YOCTO_ZTYPE == 'c'
		typedef cplx_t z_type;
#		endif
		
		
#	define REAL_DIG        FLT_DIG
#	define REAL_EPSILON    FLT_EPSILON
#	define REAL_MANT_DIG   FLT_MANT_DIG
#	define REAL_MAX        FLT_MAX
#	define REAL_MAX_10_EXP FLT_MAX_10_EXP
#	define REAL_MAX_EXP    FLT_MAX_EXP
#	define REAL_MIN        FLT_MIN
#	define REAL_MIN_10_EXP FLT_MIN_10_EXP
#	define REAL_MIN_EXP    FLT_MIN_EXP
#	define REAL(X)         X##f
#	define REAL_PI         3.1415927410125732421875f
#	define REAL_TWO_PI     6.283185482025146484375f
		
		
		
#	endif // s or c
		
		
		//======================================================================
		//
		// double precision types
		//
		//======================================================================
		
#	if (YOCTO_ZTYPE == 'd') || (YOCTO_ZTYPE == 'q' ) 
		typedef double          real_t;
		typedef complex<real_t> cplx_t;
		
#		if YOCTO_ZTYPE == 'd'
		typedef real_t z_type;
#		endif
		
#		if YOCTO_ZTYPE == 'q'
		typedef cplx_t z_type;
#		endif
		
		
#	define REAL_DIG        DBL_DIG
#	define REAL_EPSILON    DBL_EPSILON
#	define REAL_MANT_DIG   DBL_MANT_DIG
#	define REAL_MAX        DBL_MAX
#	define REAL_MAX_10_EXP DBL_MAX_10_EXP
#	define REAL_MAX_EXP    DBL_MAX_EXP
#	define REAL_MIN        DBL_MIN
#	define REAL_MIN_10_EXP DBL_MIN_10_EXP
#	define REAL_MIN_EXP    DBL_MIN_EXP
#	define REAL(X)         X
#	define REAL_PI         3.141592653589793115997963468544185161590576171875
#	define REAL_TWO_PI     6.28318530717958623199592693708837032318115234375
		
#	endif // d or q
		
		
#	if YOCTO_ZTYPE == 'i'
		typedef unit_t real_t;
#	endif
		
#if !defined(YOCTO_ZTYPE)
#	error	"YOCTO_ZTYPE is not defined!"
#endif
		
	}
}


#endif
