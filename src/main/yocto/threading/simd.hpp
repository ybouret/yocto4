#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/threading/context.hpp"
#include "yocto/threading/condition.hpp"

namespace yocto
{
    namespace threading
    {
        
        //! Single Instruction Multiple Data framework
        class SIMD : public layout
        {
        public:
            explicit SIMD(); //!< use layout API (aka YOCTO_THREADING...)
            explicit SIMD(size_t num_threads, size_t thread_offset=0); //!< manual settings
            virtual ~SIMD() throw();

            //! get a 0..size-1 context
            context       & operator[]( size_t rank ) throw();

            //! get a 0..size-t const context
            const context & operator[]( const size_t rank) const throw();

            //! execute #threads copy of kernel with different contexts
            void operator()( context::kernel &K );

            //! create a DATATYPE inside context::vslot, no args, no ctx
            template <typename DATATYPE>
            inline void create()
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    self[i]. template make<DATATYPE>();
                }
            }

            //! create a DATATYPE(args) inside context::vslot, 1 arg, no ctx
            template <typename DATATYPE,typename ARG>
            inline void create( typename type_traits<ARG>::parameter_type arg )
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    self[i]. template build<DATATYPE,ARG>(arg);
                }
            }

            //! create a DATATYPE(args) inside context::vslot, 2 args, no ctx
            template <typename DATATYPE,typename ARG1, typename ARG2>
            inline void create(typename type_traits<ARG1>::parameter_type arg1,
                               typename type_traits<ARG2>::parameter_type arg2)
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    self[i]. template build<DATATYPE,ARG1,ARG2>(arg1,arg2);
                }
            }

            //! create a DATATYPE(args) inside context::vslot, 3 args, no ctx
            template <typename DATATYPE,typename ARG1, typename ARG2, typename ARG3>
            inline void create(typename type_traits<ARG1>::parameter_type arg1,
                               typename type_traits<ARG2>::parameter_type arg2,
                               typename type_traits<ARG3>::parameter_type arg3
                               )
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    self[i]. template build<DATATYPE,ARG1,ARG2,ARG3>(arg1,arg2,arg3);
                }
            }


            //! create a DATATYPE inside context::vslot, self context, no args
            template <typename DATATYPE>
            inline void create_from_context()
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    context &ctx = self[i];
                    ctx. template build<DATATYPE,const context &>(ctx);
                }
            }

            //! create a DATATYPE inside context::vslot, self context, 1 arg
            template <typename DATATYPE,typename ARG>
            inline void create_from_context(typename type_traits<ARG>::parameter_type arg )
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    context &ctx = self[i];
                    ctx. template build<DATATYPE,const context &,ARG>(ctx,arg);
                }
            }

            //! create a DATATYPE inside context::vslot, self context, 2 args
            template <typename DATATYPE,typename ARG1,typename ARG2>
            inline void create_from_context(typename type_traits<ARG1>::parameter_type arg1,
                                            typename type_traits<ARG2>::parameter_type arg2)
            {
                SIMD &self = *this;
                for(size_t i=0;i<size;++i)
                {
                    context &ctx = self[i];
                    ctx. template build<DATATYPE,const context &,ARG1,ARG2>(ctx,arg1,arg2);
                }
            }

            void free()    throw();
            void release() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
            threads    workers; //!< internal threads
        public:
            mutex     &access;  //!< shared access for everyone
        private:
            condition        enter;  //!< cycle synchro
            condition        leave;  //!< main thread wait on it
            size_t           ready;  //!< availability
            size_t           activ;  //!< detect end of work
            bool             stop;   //!< to shutdown threads
            size_t           built;  //!< global count for terminate
            context::kernel *kproc;  //!< what to do during the cycle
            
            size_t   wlen; //!< extra memory size
            void    *wksp; //!< extra memory data
            
            void initialize();
            void terminate() throw();
            
            void create_contexts();
            void delete_contexts() throw();
            
            static void subroutine( void * ) throw();
            void   engine( size_t rank ) throw();
        };
    }
}

#endif
