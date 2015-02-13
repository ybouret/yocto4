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
            explicit SIMD(); //!< use layout
            explicit SIMD(size_t num_threads, size_t thread_offset=0); //!< manual settings
            virtual ~SIMD() throw();
            
            //! a Kernel: context dependent function
            typedef context::kernel Kernel;
            
            
            context       & operator[]( size_t rank ) throw();
            const context & operator[]( const size_t rank) const throw();
            
            //! execute #threads copy of kernel with different contexts
            void operator()( Kernel &K );
            
            //! make windows in contexts data
            template <typename WINDOW>
            inline void dispatch( size_t length, size_t offset )
            {
                context::dispatch<SIMD,WINDOW>(*this,length,offset);
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
            threads    workers; //!< internal threads
        public:
            mutex     &access;  //!< shared access for everyone
        private:
            condition  enter;  //!< cycle synchro
            condition  leave;  //!< main thread wait on it
            size_t     ready;  //!< availability
            size_t     activ;  //!< detect end of work
            bool       stop;   //!< to shutdown threads
            size_t     built;  //!< global count for terminate
            Kernel    *kproc;  //!< what to do during the cycle
            
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
