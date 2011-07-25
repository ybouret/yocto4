#include "yocto/hashing/hash64.hpp"
#include "yocto/code/unroll.hpp"


namespace yocto
{
	namespace hashing
	{
		static uint32_t bj_hash32( uint32_t a)
		{
			a = (a+0x7ed55d16) + (a<<12);
			a = (a^0xc761c23c) ^ (a>>19);
			a = (a+0x165667b1) + (a<<5);
			a = (a+0xd3a2646c) ^ (a<<9);
			a = (a+0xfd7046c5) + (a<<3);
			a = (a^0xb55a4f09) ^ (a>>16);
			return a;
		}
		
		void hash64:: BJ( uint32_t *lword, uint32_t *rword ) throw()
		{
#if defined(__ICC)
#pragma warning ( disable : 280 )
#endif
			YOCTO_LOOP_(4,
						const uint32_t swp = (*rword);
						(*rword) = bj_hash32(*rword) ^ (*lword);
						(*lword) = swp );
		}
		
#define _YCE_NR_HASH64(i) \
{\
const uint32_t it = (*rword);          \
const uint32_t ia = (it) ^ X##i;       \
const uint32_t il = ia & 0xffff;       \
const uint32_t ih = ia >>  0x10;       \
const uint32_t ib = (il*il) + ~(ih*ih);\
(*rword) = (*lword) ^ (( ( (ib >> 0x10) | ((ib & 0xffff) << 0x10)) ^ Y##i)+ (il*ih) );\
(*lword) = it;\
}
		
		void hash64:: NR( uint32_t *lword, uint32_t *rword ) throw()
		{
			
			static const uint32_t X0 = 0xbaa96887L;
			static const uint32_t X1 = 0x1e17d32cL;
			static const uint32_t X2 = 0x03bcdc3cL;
			static const uint32_t X3 = 0x0f33d1b2L;
			
			static const uint32_t Y0 = 0x4b0f3b58L;
			static const uint32_t Y1 = 0xe874f0c3L;
			static const uint32_t Y2 = 0x6955c5a6L;
			static const uint32_t Y3 = 0x55a7ca46L;
			
			_YCE_NR_HASH64(0) _YCE_NR_HASH64(1) _YCE_NR_HASH64(2) _YCE_NR_HASH64(3)
			
			
		}
		
		/* #include <string.h> */
		
		size_t hash64::of( size_t i, hash64::proc h) throw()
		{
			union {
				uint32_t dw[2];
				size_t   sz;
			} wksp = { {0,0} } ;
			
			assert( sizeof(size_t) <= sizeof( uint32_t[2] ) );
			assert( h != NULL );
			
			wksp.sz = i;
			h( & wksp.dw[0], & wksp.dw[1] );
			return wksp.sz;
		}
		
		uint64_t hash64::mix64(const uint64_t x, hash64::proc h) throw()
		{
			union  {
				uint64_t qw;
				uint32_t dw[2];
			} wksp = { x };
			assert( h != NULL );
			h( & wksp.dw[0], & wksp.dw[1] );
			return wksp.qw;
		}
		
	}
}


