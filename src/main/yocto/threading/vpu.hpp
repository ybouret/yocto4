#ifndef YOCTO_THREADING_VPU_INCLUDED
#define YOCTO_THREADING_VPU_INCLUDED 1

#include "yocto/threading/crew.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/slots.hpp"

namespace yocto
{
    namespace threading
    {

        class vpu
        {
        public:
            typedef shared_ptr<kernel_executor> kexec_ptr;
            virtual ~vpu() throw();

            const size_t cores;

        protected:
            explicit vpu(const kexec_ptr &kxp) throw();
            
        private:
            kexec_ptr simd;
            kernel    call;
            YOCTO_DISABLE_COPY_AND_ASSIGN(vpu);
            void __call( context & ) throw();
        };


        template <typename T>
        class processing_unit :
        public vpu,
        slots_of<T>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;

            typedef slots_of<T> slots_type;
            virtual ~processing_unit() throw() {}

            explicit processing_unit(const kexec_ptr &kxp) :
            vpu(kxp), slots_type(cores)
            {
            }





        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(processing_unit);
        };
        
        
    }
}

#endif
