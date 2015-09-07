#include "yocto/threading/crew.hpp"
#include <iostream>

namespace yocto
{
    namespace threading
    {
        crew::context:: ~context() throw() {}

        crew::context:: context(const size_t the_rank,
                                const size_t the_size,
                                lockable    &the_lock) throw() :
        rank(the_rank),
        size(the_size),
        access(the_lock),
        indx(rank+1),
        priv(0)
        {
            assert(size>0);
            assert(rank<size);
        }


        crew:: single_context:: ~single_context() throw() {}

        crew:: single_context:: single_context() throw() :
        faked_lock(),
        context(0,1,*this)
        {
        }

    }

}

namespace yocto
{
    namespace threading
    {

#define Y_THREADING_CREW_CTOR() \
workers("crew",size),           \
access(workers.access),         \
ready(0),                       \
cycle(),                        \
synch(),                        \
contexts(size),                 \
dying(false)
        
        crew:: crew() : layout(),
        Y_THREADING_CREW_CTOR()
        {
            init();
        }

        crew:: crew(const size_t num_cpus, const size_t cpu_start) :
        layout(num_cpus,cpu_start),
        Y_THREADING_CREW_CTOR()
        {
            init();
        }

        crew:: ~crew() throw()
        {
            quit();
        }
    }
}

namespace yocto
{
    namespace threading
    {

        void crew:: init()
        {


            try
            {
                //______________________________________________________________
                //
                // initializing threads
                //______________________________________________________________
                for(size_t rank=0;rank<size;++rank)
                {
                    //-- build context
                    contexts.append<size_t,size_t,lockable&>(rank,size,access);
                    context &ctx = contexts[rank];
                    ctx.priv = this;

                    //-- create thread from context
                    workers.launch(worker_call,&ctx);
                }

                //______________________________________________________________
                //
                // wait for first synchronization
                //______________________________________________________________
                while(true)
                {
                    if(access.try_lock())
                    {
                        if(ready>=size)
                        {
                            std::cerr << "[crew] synchronized !" << std::endl;
                            // main thread on root CPU
                            std::cerr << "[crew] assigning main thread:" << std::endl;
                            std::cerr << "[crew] "; assign_current_thread_on( cpu_index_of(0) );

                            // regular dispatch of workers
                            std::cerr << "[crew] assigning workers thread:" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr->next; thr=thr->next)
                            {
                                std::cerr << "[crew] "; thr->on_cpu( cpu_index_of(iThread++) );
                            }

                            access.unlock();
                            break;
                        }
                        else
                        {
                            access.unlock();
                        }
                    }
                }


            }
            catch(...)
            {
                quit();
                throw;
            }

        }

        void crew:: quit() throw()
        {
            access.lock();
            std::cerr << "[crew] turing off..." << std::endl;
            (bool&)dying = true;
            access.unlock();

            //__________________________________________________________________
            //
            // waiting for current cycle to finnish
            //__________________________________________________________________

            access.lock();

            access.unlock();

            //__________________________________________________________________
            //
            // last cycle: on a dying crew...
            //__________________________________________________________________
            cycle.broadcast();

            //__________________________________________________________________
            //
            // waiting for threads to return
            //__________________________________________________________________
            workers.finish();
            std::cerr << "[crew] halted." << std::endl;
        }

        void crew::worker_call(void *args) throw()
        {
            assert(args);
            context &Cntx  = *static_cast<context *>(args); assert(Cntx.priv);
            crew    &Crew = *static_cast<crew    *>(Cntx.priv);
            Crew.worker_loop(Cntx);
        }

    }

}


namespace yocto
{
    namespace threading
    {
        void crew:: worker_loop(const context &ctx) throw()
        {
            //__________________________________________________________________
            //
            // go to first synchronization
            //__________________________________________________________________
            access.lock();
            //std::cerr << "[crew] init " << ctx.size << "." << ctx.rank << std::endl;
            ++ready;

            //__________________________________________________________________
            //
            // wait on a LOCKED accces
            //__________________________________________________________________
        WAIT_FOR_CYCLE:
            std::cerr << "[crew] " << ctx.size << "." << ctx.rank << ": waiting, #ready=" << ready << std::endl;
            cycle.wait(access);

            //__________________________________________________________________
            //
            // woke up, access is LOCKED
            //__________________________________________________________________
            if(dying)
            {
                std::cerr << "[crew] end " << ctx.size << "." << ctx.rank << std::endl;
                access.unlock();
                return;
            }

            //__________________________________________________________________
            //
            // Execute the current kernel
            //__________________________________________________________________
            assert(ready>0);
            --ready;

            ++ready;

            goto WAIT_FOR_CYCLE;
        }


    }

}


