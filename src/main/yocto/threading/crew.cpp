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
dying(false),                   \
kproc(0),                       \
cycle(),                        \
synch(),                        \
contexts(size),                 \
failure(0)

        crew:: crew(bool setVerbose) : layout(setVerbose),
        Y_THREADING_CREW_CTOR()
        {
            init();
        }
        
        crew:: crew(const size_t num_cpus, const size_t cpu_start,bool setVerbose) :
        layout(num_cpus,cpu_start,setVerbose),
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
                if(verbose)
                {
                    YOCTO_LOCK(access);
                    std::cerr << "[crew] creating #thread=" << size << ", #ready=" << ready << std::endl;
                }
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
                            assert(size==workers.size);
                            if(verbose)
                            {
                                std::cerr << "[crew] synchronized !" << std::endl;
                                // main thread on root CPU
                                std::cerr << "[crew] assigning main thread:" << std::endl;
                                std::cerr << "[crew] ";
                            }
                            assign_current_thread_on( cpu_index_of(0) );
                            
                            // regular dispatch of workers
                            if(verbose) std::cerr << "[crew] assigning workers thread:" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr; thr=thr->next)
                            {
                                if(verbose) std::cerr << "[crew] ";
                                thr->on_cpu( cpu_index_of(iThread++) );
                            }
                            if(verbose) std::cerr << "[crew] is ready!" << std::endl << std::endl;
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
            if(verbose) std::cerr << std::endl << "[crew] shuting down..." << std::endl;
            (bool&)dying = true;
            kproc        = NULL;
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
            if(verbose) std::cerr << "[crew] halted." << std::endl;
        }
        
        void crew::worker_call(void *args) throw()
        {
            assert(args);
            context &Cntx  = *static_cast<context *>(args); assert(Cntx.priv);
            crew    &Crew  = *static_cast<crew    *>(Cntx.priv);
            Crew.worker_loop(Cntx);
        }
        
    }
    
}


namespace yocto
{
    namespace threading
    {
        void crew:: worker_loop(context &ctx) throw()
        {
            //__________________________________________________________________
            //
            // go to first synchronization
            //__________________________________________________________________
            access.lock();
            if(verbose) std::cerr << "[crew] init " << ctx.size << "." << ctx.rank << std::endl;
            ++ready;
            
            //__________________________________________________________________
            //
            // wait on a LOCKED accces
            //__________________________________________________________________
        WAIT_FOR_CYCLE:
            //std::cerr << "[crew] " << ctx.size << "." << ctx.rank << ": waiting, #ready=" << ready << std::endl;
            cycle.wait(access);
            
            //__________________________________________________________________
            //
            // woke up, access is LOCKED
            //__________________________________________________________________
            if(dying)
            {
                assert(NULL==kproc);
                if(verbose) std::cerr << "[crew] end " << ctx.size << "." << ctx.rank << std::endl;
                access.unlock();
                return;
            }
            
            //__________________________________________________________________
            //
            // Execute the current kernel
            //__________________________________________________________________
            assert(kproc!=NULL);
            access.unlock();
            
            try
            {
                (*kproc)(ctx);
            }
            catch(...)
            {
                //TODO: send a message ?
                YOCTO_LOCK(access);
                (size_t&)failure = ctx.indx;
            }
            
            access.lock();
            ++ready;
            //std::cerr << "[crew] ok " << ctx.size << "." << ctx.rank << std::endl;
            if(ready>=size)
            {
                synch.broadcast();
            }
            goto WAIT_FOR_CYCLE;
        }
        
        
    }
    
}


namespace yocto
{
    namespace threading
    {
        void crew:: operator()( kernel &K ) throw()
        {
            access.lock();         // lock access
            assert(size==ready);   // must be true here
            kproc = &K;            // local link
            ready = 0;             // global counter
            (size_t&)failure = 0;  // clean up flag
            cycle.broadcast();     // would start all threads
            synch.wait(access);    // unlock access => start threads => come back LOCKED
            assert(size==ready);   // must be true here
            access.unlock();       // and unlock for next cycle...
        }
    }
}

