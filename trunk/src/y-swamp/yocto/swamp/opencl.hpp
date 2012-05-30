#ifndef YOCTO_SWAMP_OPENCL_INCLUDED
#define YOCTO_SWAMP_OPENCL_INCLUDED 1

#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/command-queue.hpp"
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
            
            inline void Write(ocl::CommandQueue  &queue, 
                              cl_bool             blocking_write,
                              cl_uint             num_events_in_wait_list,
                              const cl_event *    event_wait_list,
                              cl_event *          event )
            {
                queue.EnqueueWriteBuffer(*this, 
                                         blocking_write, 
                                         0,
                                         SIZE, 
                                         ptr_,
                                         num_events_in_wait_list,
                                         event_wait_list,
                                         event);
            }
            
            inline void Read(ocl::CommandQueue  &queue, 
                             cl_bool             blocking_read,
                             cl_uint             num_events_in_wait_list,
                             const cl_event *    event_wait_list,
                             cl_event *          event )
            {
                queue.EnqueueReadBuffer(*this, 
                                        blocking_read, 
                                        0,
                                        SIZE, 
                                        ptr_,
                                        num_events_in_wait_list,
                                        event_wait_list,
                                        event);
            }
            
            
        private:
            varray::ptr arr_;
            void       *ptr_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(OCL_Buffer);
        };
        
    }
    
}

#endif
