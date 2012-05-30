#ifndef YOCTO_SWAMP_OPENCL_INCLUDED
#define YOCTO_SWAMP_OPENCL_INCLUDED 1

#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/buffer.hpp"
#include "yocto/swamp/common.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        
        class OCL_Buffer : public ocl::Buffer
        {
        public:
            explicit OCL_Buffer( const varray::ptr &arr, const ocl::Context  &ctx) : 
            ocl::Buffer(ctx, CL_MEM_READ_WRITE, arr->handle()->bytes, NULL ),
            arr_(arr),
            ptr_((void*)(arr_->handle()->address_of(0)))
            {
            }
            
            virtual ~OCL_Buffer() throw() {}
            
        private:
            varray::ptr arr_;
            void       *ptr_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(OCL_Buffer);
        };
        
    }
    
}

#endif
