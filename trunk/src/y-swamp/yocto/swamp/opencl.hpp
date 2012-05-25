#ifndef YOCTO_SWAMP_OPENCL_INCLUDED
#define YOCTO_SWAMP_OPENCL_INCLUDED 1

#include "yocto/ocl/buffer.hpp"
#include "yocto/swamp/common.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        template <typename T, typename LAYOUT>
        class OCL_Buffer : public ocl::Buffer
        {
        public:
            typedef linear<T, LAYOUT> linear_type;
            explicit OCL_Buffer( const string &name, const ocl::Context  &ctx);
            virtual ~OCL_Buffer() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(OCL_Buffer);
        };
        
    }
    
}

#endif
