#ifndef YOCTO_HASHING_RMD160_INCLUDED
#define YOCTO_HASHING_RMD160_INCLUDED 1


#include "yocto/hashing/function.hpp"
#include "yocto/hashing/rmd.hpp"

namespace yocto
{
    namespace hashing
    {
        
        class rmd160 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(rmd160,20,64);
			static const size_t RMDsize=160;
            
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rmd160);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
