#ifndef YOCTO_CRYPTO_BC_TEA_INCLUDED
#define YOCTO_CRYPTO_BC_TEA_INCLUDED 1

#include "yocto/os.hpp"


namespace yocto {
	
	namespace crypto {
		/* The length of the data should be at least 8
		 // bytes (dataLen >= 2 uint32_t), else the algorithm will not work.
		 
		 // The key should be 128 bit, or 16 bytes (4 uint32_t)
		 
		 // The memory pointed to by data will be modified by
		 // the functions.
		 */
		void tea_encrypt( uint32_t *dw_reg, const size_t dw_num, const uint32_t *k) throw();
		void tea_decrypt( uint32_t *dw_reg, const size_t dw_num, const uint32_t *k) throw();
	}
}
#endif
