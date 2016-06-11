#ifndef YOCTO_THREADING_VPU_INCLUDED
#define YOCTO_THREADING_VPU_INCLUDED 1

#include "yocto/threading/crew.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/slots.hpp"

namespace yocto
{
    namespace threading
    {

        class VPU
        {
        public:
            typedef shared_ptr<kernel_executor> kexec_ptr;
            virtual ~VPU() throw();

        protected:
            explicit VPU(const kexec_ptr &kxp) throw();

        private:
            kexec_ptr SIMD;
            kernel    CALL;
            YOCTO_DISABLE_COPY_AND_ASSIGN(VPU);
            void call( context & ) throw();
        };


        template <typename T>
        class processing_unit :
        public VPU,
        slots_of<T>
        {
        public:
            typedef slots_of<T> slots_type;
            virtual ~processing_unit() throw() {}

            explicit processing_unit( const kexec_ptr &kxp ) :
            VPU(kxp),
            slots_type(kxp->num_threads())
            {
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(processing_unit);
        };


    }
}

#endif
