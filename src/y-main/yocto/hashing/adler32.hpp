#ifndef YOCTO_HASHING_ADLER32_INCLUDED
#define YOCTO_HASHING_ADLER32_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{

		class adler32 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(4,4);
			explicit adler32() throw();
			virtual ~adler32() throw();

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(adler32);
		};

	}
}

#endif
