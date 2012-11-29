#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    
    namespace threading
    {
        class crew : public layout
        {
        public:
            
            class context
            {
            public:
                const size_t rank;
                const size_t indx;
                const size_t size;
                lockable    &access;
                
                context( size_t r, size_t s, lockable &lock_ref) throw();
                ~context() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
            };
            
            typedef functor<void,TL1(context&)> task;
            
            explicit crew();
            explicit crew(size_t num_threads,size_t off_threads);
            virtual ~crew() throw();
            void run( task &sub ) throw();
            
            //! automatically dispatch indices
            /**
             \param workers array of pointers to a class with some start/count/final members
             \param offset  computing task offset
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
            mutex      access; //!< shared mutex
            condition  enter;  //!< cycle synchro
            condition  leave;  //!< main thread wait on it
            size_t     ready;  //!< availability
            size_t     activ;  //!< detect end of work
            size_t     built;  //!< for terminate
            task      *proc;   //!< job to do
            bool       stop;   //!< to shutdown threads
            size_t     wlen;   //!< memory length
            void      *wksp;   //!< memory workspace
            size_t     nthr;   //!< for ctor/dtor
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            void   initialize();
            void   terminate() throw();
            size_t cpu_index( size_t i, size_t n ) const throw();
            void   place();
            void   engine( size_t rank ) throw();
            
            static void launch(void *args) throw();
        };
        
    }
    
}

#endif
