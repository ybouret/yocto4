#ifndef YOCTO_THREADING_ENGINE_INCLUDED
#define YOCTO_THREADING_ENGINE_INCLUDED 1

#include "yocto/threading/threads.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace threading
    {

        
        class engine : public layout
        {
        public:
            explicit engine();
            explicit engine(const size_t num_threads, const size_t threads_offset=0);
            virtual ~engine() throw();

        private:
            threads   workers;
            condition more_work;
            condition work_done;

        public:
            mutex &access;
            
        private:
            bool   dying;
            size_t ready;
            YOCTO_DISABLE_COPY_AND_ASSIGN(engine);

            void init();
            void quit() throw();
            
            static void worker_call(void *args) throw();
            void        worker_loop() throw();
            
            static void master_call(void *args) throw();
            void        master_loop() throw();
            
        };

    }
}


#endif
