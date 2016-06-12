#ifndef YOCTO_THREADING_VPU_INCLUDED
#define YOCTO_THREADING_VPU_INCLUDED 1

#include "yocto/threading/crew.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/slots.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace threading
    {

        //! base class for virtual processing unit
        class vpu
        {
        public:
            typedef shared_ptr<kernel_executor> kexec_ptr;
            virtual ~vpu() throw();

            const size_t cores;

        protected:
            kexec_ptr simd;

            explicit vpu(const kexec_ptr &kxp) throw();

            YOCTO_DISABLE_COPY_AND_ASSIGN(vpu);
        };


        //! will associate one processor per core
        template <typename T>
        class processing_unit :
        public vpu,
        public slots_of<T>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;

            typedef slots_of<T> slots_type;
            virtual ~processing_unit() throw() {}

            explicit processing_unit(const kexec_ptr &kxp) :
            vpu(kxp), slots_type(cores),
            source(0), target(0)
            {
            }

            //! prepare a kernel to act on array<U>
            template <typename U>
            inline void compile()
            {
                code.reset( new kernel(this, & processing_unit<T>::call1<U> ) );
            }

            //! prepare a kernel to act on array<U> <= array<V>
            template <typename U,typename V>
            inline void compile()
            {
                code.reset( new kernel(this, & processing_unit<T>::call2<U,V> ) );
            }
            
            
            template <typename U>
            inline void call( array<U> &arrU, void *args) throw()
            {
                assert( code.is_valid() );
                source = target = &arrU;
                params = args;
                (*simd)(*code);
            }

            template <typename U, typename V>
            inline void call( array<U> &arrU, const array<V> &arrV , void *args) throw()
            {
                assert( code.is_valid() );
                target = &arrU;
                source = &arrV;
                params = args;
                (*simd)(*code);
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(processing_unit);
            auto_ptr<kernel> code;
            const void      *source;
            void            *target;
            void            *params;
            
            template <typename U>
            inline void call1( context &ctx ) throw()
            {
                assert(source); assert(target);
                assert(this->size==cores);
                slots_type &self = *this;
                array<U>   &arrU = *static_cast< array<U> *>(target);
                self[ctx.rank].run(ctx,arrU,params);
            }
            
            template <typename U, typename V>
            inline void call2( context &ctx ) throw()
            {
                assert(source); assert(target);
                assert(this->size==cores);
                slots_type &self = *this;
                array<U>       &arrU = *static_cast< array<U>       *>(target);
                const array<V> &arrV = *static_cast< const array<V> *>(source);
                self[ctx.rank].run(ctx,arrU,arrV,params);
            }

            
        };
        
        
    }
}

#endif
