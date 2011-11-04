#include "yocto/code/rand.hpp"
#include "yocto/wtime.hpp"
namespace yocto
{
#if defined(__ICC)
#pragma warning ( disable : 1419 )
#endif
	extern uint32_t ihash32( uint32_t );
	void urand:: wseed()
	{
		reset( wtime::seed() );
	}
	
	urand:: urand() throw() :
	w(521288629), 
	z(362436069),
	jsr(123456789), 
	jcong(380116160)
	{
	}
	
	urand:: ~urand() throw()
	{
	}
	
	urand:: urand( uint32_t seed ) throw() :
	w(0),z(0),jsr(0),jcong(0)
	{
		reset( seed );
	}
	
	
	void urand:: reset( uint32_t seed ) throw()
	{
		w     = ( seed = ihash32(seed) );
		z     = ( seed = ihash32(seed) );
		jsr   = ( seed = ihash32(seed) );
		jcong =  ihash32(seed);
	}
	
	uint32_t urand:: MWC() throw()
	{
		z = 36969 * (z & 65535) + (z >> 16);
		w = 18000 * (w & 65535) + (w >> 16);
		return (z << 16) + w;  /* 32-bit result */
	}
	
	uint32_t urand:: SHR3( ) throw()
	{
		jsr^=(jsr<<17);
		jsr^=(jsr>>13);
		jsr^=(jsr<< 5);
		return jsr;
	}
	
	
	uint32_t urand:: CONG( ) throw()
	{
		jcong = 69069 * jcong+1234567;
		return jcong;
	}
	
	uint32_t urand:: KISS() throw()
	{
		return ( MWC() ^ CONG() ) + SHR3();
	}
	
	template <>
	uint32_t urand:: get<uint32_t>() throw() { return KISS(); }
	
	template <>
	double urand:: get<double>() throw() 
	{ 
		static const double fac= 1.0 / 4294967296.0;
		return ( 0.5 + static_cast<double>( KISS() ) ) * fac;
	}
	
	template <>
	float urand:: get<float>() throw() 
	{ 
		static const float fac= 1.0f / 4294967296.0f;
		return ( 0.5f + float( KISS() ) ) * fac;
	}
	
	static   urand   __rand;
	urand  & _rand = __rand;
	
}
