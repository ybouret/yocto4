#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    
    namespace threading
    {
        //! make a crew of threads
        class crew : public layout
        {
        public:
            
            //! context of current thread
            class context
            {
            public:
                const size_t rank;   //!< 0..size-1
                const size_t indx;   //!< rank+1, for information
                const size_t size;   //!< size of the crew
                lockable    &access; //!< common mutex for synchronization
                
                context( size_t r, size_t s, lockable &lock_ref) throw();
                ~context() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
            };
            
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
            
            //! automatically dispatch indices
            /**
             \param workers array of pointers to a class with some start/count/final members
             \param offset  computing task offset (integral type)
             \param length  computing length
             */
            template <class PTR_ARRAY,class U>
            void dispatch( PTR_ARRAY &workers, U offset, size_t length ) const throw()
            {
                assert( workers.size() == size );
                for( size_t rank=0;rank<size;++rank)
                {
                    const size_t i     = rank+1;
                    const size_t count = length / ( size-rank );
                    workers[i]->start  = offset;
                    workers[i]->count  = count;
                    offset += count;
                    length -= count;
                    workers[i]->final = offset-1;
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
        
    }
    
}

#endif
