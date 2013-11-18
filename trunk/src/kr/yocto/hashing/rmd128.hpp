#ifndef YOCTO_HASHING_RMD128_INCLUDED
#define YOCTO_HASHING_RMD128_INCLUDED 1


#include "yocto/hashing/function.hpp"
#include "yocto/hashing/rmd.hpp"

namespace yocto
{
    namespace hashing
    {
        
        class rmd128 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(rmd128,16,64);
			static const size_t RMDsize=128;
            
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rmd128);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
