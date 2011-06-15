#ifndef YOCTO_CRYPTO_BC_GRAY_INCLUDED
#define YOCTO_CRYPTO_BC_GRAY_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto {

	namespace crypto {

		template <typename T>
		struct gray
		{
			static T encode( T n ) throw();
			static T decode( T n ) throw();
		};

		

	}

}

#endif
