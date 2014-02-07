#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/threading/context.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    
    namespace threading
    {
        
#if 0
        //! make a crew of threads
        class crew : public layout
        {
        public:
            
            
            
            //! task to be run per context
            typedef functor<void,TL1(context&)> task;
            
            //! make a crew with hardware #procs, 1:1 placement
            explicit crew();
            
            //! make a crew with #num_threads, starting at CPU #off_threads
            explicit crew(size_t num_threads,size_t off_threads);
            
            //! clean up
            virtual ~crew() throw();
            
            //! call the same task for every context/thread
            void run( task &sub ) throw();
            
            context &       operator[](size_t rank) throw();
            const context & operator[](size_t rank) const throw();
            
            //! make windows in contexts data
            template <typename WINDOW>
            inline void dispatch( size_t length, size_t offset )
            {
                crew &self = *this;
                for( size_t rank=0;rank<size;++rank)
                {
                    context       &ctx = self[rank];
                    const WINDOW   win(ctx,length,offset);
                    ctx.make<WINDOW>(win);
                }
            }
            
            
        private:
            mutex        access; //!< shared mutex
            condition    enter;  //!< cycle synchro
            condition    leave;  //!< main thread wait on it
            size_t       ready;  //!< availability
            size_t       activ;  //!< detect end of work
            size_t       built;  //!< for terminate
            task        *proc;   //!< job to do
            bool         stop;   //!< to shutdown threads
            const size_t woff;   //!< offset for contexts
            size_t       wlen;   //!< memory length for internal data + contexts
            void        *wksp;   //!< memory workspace
            size_t       nthr;   //!< for ctor/dtor
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            void   initialize();
            void   terminate() throw();
            size_t cpu_index( size_t i, size_t n ) const throw();
            void   place();
            void   engine( size_t rank ) throw();
            
            static void launch(void *args) throw();
            
            void create_contexts() throw();
            void delete_contexts() throw();
        };
#endif

    }
}

#endif
