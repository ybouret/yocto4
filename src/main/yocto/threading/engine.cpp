#include "yocto/threading/engine.hpp"
#include <iostream>

namespace yocto
{
    namespace threading
    {
#define Y_LOCKED_ENGINE(CODE) do { \
YOCTO_LOCK(access);                \
CODE;                              \
} while(false)
        
#define Y_THREADING_ENGINE_CTOR() \
workers("engine"),                \
more_work(),                      \
work_done(),                      \
access(workers.access),           \
dying(false),                     \
ready(0)
        
        engine:: engine() : layout(),
        Y_THREADING_ENGINE_CTOR()
        {
            init();
        }
        
        engine:: engine(const size_t num_threads, const size_t threads_offset) :
        layout(num_threads,threads_offset),
        Y_THREADING_ENGINE_CTOR()
        {
            init();
        }
        
        
        engine:: ~engine() throw()
        {
            quit();
        }
        
        
        void engine:: init()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] init..." << std::endl);
            
            try
            {
                
                //______________________________________________________________
                //
                // prepare all worker threads:0..size-1
                //______________________________________________________________
                for(size_t i=0;i<size;++i)
                {
                    workers.launch(worker_call,this);
                }
                
                //______________________________________________________________
                //
                // prepare master thread
                //______________________________________________________________
                workers.launch(master_call,this);
                
                //______________________________________________________________
                //
                // wait for first synchronization
                //______________________________________________________________
                while(true)
                {
                    if(access.try_lock())
                    {
                        if(ready<=size)
                        {
                            access.unlock();
                        }
                        else
                        {
                            assert(1+size==ready);
                            std::cerr << "[engine] threads are synchronized" << std::endl;

                            //__________________________________________________
                            //
                            // threads placements
                            //__________________________________________________
                            std::cerr << "[engine] assigning main thread" << std::endl;
                            assign_current_thread_on( cpu_index_of(0) );

                            std::cerr << "[engine] assigning workers thread" << std::endl;
                            size_t iThread = 0;
                            for(thread *thr = workers.head; thr->next; thr=thr->next)
                            {
                                thr->on_cpu( cpu_index_of(iThread++) );
                            }
                            std::cerr << "[engine] assigning MASTER thread" << std::endl;
                            workers.tail->on_cpu( cpu_index_of(0) );

                            std::cerr << "[engine] all threads are ready" << std::endl << std::endl;
                            access.unlock();
                            break;
                        }
                    }
                }
                Y_LOCKED_ENGINE(std::cerr << "[engine] threads are ready" << std::endl);
                
                
            }
            catch(...)
            {
                quit();
                throw;
            }
        }
        
        
        void engine:: quit() throw()
        {
            Y_LOCKED_ENGINE(std::cerr << "[engine] quit..." << std::endl);
            Y_LOCKED_ENGINE(dying=true);
            
            //__________________________________________________________________
            //
            // ok, all jobs are done...
            //__________________________________________________________________
            
            more_work.broadcast();
            work_done.broadcast();
            //__________________________________________________________________
            //
            // and finally finnish all threads
            //__________________________________________________________________
            Y_LOCKED_ENGINE(std::cerr << "[engine] wait for all threads to return..." << std::endl);
            workers.finish();
            
            Y_LOCKED_ENGINE(std::cerr << "[engine] ...and done!" << std::endl);
        }
        
        
        
        
        void engine:: worker_call(void *args) throw()
        {
            assert(args);
            static_cast<engine *>(args)->worker_loop();
        }
        
        void engine:: master_call(void *args) throw()
        {
            assert(args);
            static_cast<engine *>(args)->master_loop();
        }
    }
    
}

#include "yocto/code/bin2name.hpp"

namespace yocto
{
    namespace threading
    {
        
        void engine:: master_loop() throw()
        {
            //__________________________________________________________________
            //
            //
            // Waiting for everyone to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;
            const thread::handle_t         thread_handle = thread::get_current_handle();
            const bin2id<thread::handle_t> thread_bin_id = &thread_handle;
            const char                    *thread_name   = thread_bin_id.value;
            std::cerr << "[engine] Master name is " << thread_name << std::endl;
            //__________________________________________________________________
            //
            //
            // wait on the LOCKED access that some work was done
            //
            //__________________________________________________________________
            work_done.wait(access);

            //__________________________________________________________________
            //
            //
            // master thread is waken up !
            //
            //__________________________________________________________________
            if(dying)
            {
                std::cerr << "[engine] Master is done" << std::endl;
                access.unlock();
                return;
            }
            
            access.unlock();
        }
        
        void engine:: worker_loop() throw()
        {
            //__________________________________________________________________
            //
            //
            // Waiting for all threads to be ready
            //
            //__________________________________________________________________
            access.lock();
            ++ready;
            const thread::handle_t         thread_handle = thread::get_current_handle();
            const bin2id<thread::handle_t> thread_bin_id = &thread_handle;
            const char                    *thread_name   = thread_bin_id.value;
            std::cerr << "[engine] Worker name is " << thread_name << std::endl;

            //__________________________________________________________________
            //
            //
            // Waiting for all threads to be ready on the LOCKED access
            //
            //__________________________________________________________________
            more_work.wait(access);
            
            
            
            access.unlock();
            
        }
    }
}



