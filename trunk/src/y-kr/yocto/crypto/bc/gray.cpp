#include "yocto/crypto/bc/gray.hpp"

namespace yocto {

	namespace crypto {

		#define YOCTO_GRAY(N)       uint##N##_t
		#define YOCTO_GRAY_MAX(N) ( (4*sizeof(YOCTO_GRAY(N)) ) )



		#define YOCTO_GRAY_ENCODE(N) \
		template <> YOCTO_GRAY(N) gray<YOCTO_GRAY(N)>::encode( YOCTO_GRAY(N) n ) throw() \
		{\
			return n ^ (n>>1);\
		}

#if defined(__INTEL_COMPILER)
#pragma warning( push )
#pragma warning ( disable : 2259 )
#endif

		YOCTO_GRAY_ENCODE(8)
		YOCTO_GRAY_ENCODE(16)
		YOCTO_GRAY_ENCODE(32)
		YOCTO_GRAY_ENCODE(64)


#define YOCTO_GRAY_DECODE(N) \
template <> YOCTO_GRAY(N) gray<YOCTO_GRAY(N)>::decode( YOCTO_GRAY(N) n ) throw()\
{\
        size_t        ish = 1;\
        YOCTO_GRAY(N) ans = n;\
        for(;;)\
		{\
			const YOCTO_GRAY(N) reg = ans>>ish;\
			ans ^= reg;\
			if (reg <= 1 || ish == YOCTO_GRAY_MAX(N) ) return ans;\
			ish <<= 1;\
		}\
}

		YOCTO_GRAY_DECODE(8)
		YOCTO_GRAY_DECODE(16)
		YOCTO_GRAY_DECODE(32)
		YOCTO_GRAY_DECODE(64)


	}
}
