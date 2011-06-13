#ifndef YOCTO_HASHING_MD2_INCLUDED
#define YOCTO_HASHING_MD2_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		/* RFC 1319 */
		namespace RFC1319
		{
			struct MD2_CTX
			{
				uint8_t      state[16];    /*!< state                      */
				uint8_t      checksum[16]; /*!< checksum                   */
				size_t       count;        /*!< number of bytes, modulo 16 */
				uint8_t      buffer[16];   /*!< input buffer               */
			};
		}
		
		class md2 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(md2,16,48);
			
		private:
			RFC1319::MD2_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(md2);
		};
		
	}
}

#endif
