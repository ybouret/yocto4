#include "yocto/threading/crew.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/sys/hw.hpp"
#include <iostream>

namespace yocto
{
    
    namespace threading
    {
        
        crew::context:: ~context() throw() {}
        crew::context::  context( size_t r, size_t s, lockable &lock_ref) throw() :
        rank(r),
        indx(rank+1),
        size(s),
        access(lock_ref)
        {
        }
        
        namespace
        {
            //! lacunar class for by-hand construction
            class member
            {
            public:
                crew         *crew_p;
                size_t        rank;
                thread        work;
            private:
            };
        }
        
#define Y_CREW_CTOR()   \
access( "crew::mutex" ),\
enter(),                \
leave(),                \
ready(0),               \
activ(0),               \
built(0),               \
proc(0),                \
stop( false ),          \
wlen( size * sizeof(member) ),\
wksp( memory::kind<memory::global>::acquire(wlen) ),\
nthr(0)
        
        crew:: crew() :
        layout(),
        Y_CREW_CTOR()
        {
            initialize();
        }
        
        crew:: crew(size_t num_threads,size_t off_threads) :
        layout(num_threads,off_threads),
        Y_CREW_CTOR()
        {
            initialize();
        }
        
        
        crew:: ~crew() throw()
        {
            terminate();
        }
        
        void crew:: initialize()
        {
            try
            {
                //----------------------------------------------------------
                //
                // prepare the threads
                //
                //----------------------------------------------------------
                member *p = static_cast<member*>(wksp);
                while(nthr<size)
                {
                    member &m = p[nthr];
                    //------------------------------------------------------
                    //-- set member info
                    //------------------------------------------------------
                    m.crew_p = this;
                    m.rank   = nthr;
                    
                    //------------------------------------------------------
                    //-- launch thread
                    //------------------------------------------------------
                    new (&m.work) thread( crew::launch, &m);
                    
                    built = ++nthr;
                }
                
                //----------------------------------------------------------
                //
                // wait for first sync
                //
                //----------------------------------------------------------
                for(;;)
                {
                    if( access.try_lock() )
                    {
                        if( ready >= size)
                        {
                            std::cerr << "[crew] synchronized" << std::endl;
                            access.unlock();
                            break;
                        }
                        access.unlock();
                    }
                }
                
                
                //----------------------------------------------------------
                //
                // place them
                //
                //----------------------------------------------------------
                place();
            }
            catch(...)
            {
                terminate();
                throw;
            }
        }
        
        size_t crew:: cpu_index( size_t i, size_t n ) const throw()
        {
            const size_t j =  ( root + i % size ) % n;
            std::cerr << "              |_ assign on CPU #" << j << "/" << size << std::endl;
            return j;
        }
        
        void crew:: terminate() throw()
        {
            std::cerr << "[crew::terminate]" << std::endl;
            stop = true;
            
            for(;;)
            {
                enter.broadcast();
                scoped_lock guard(access);
                if(built<=0) break;
            }
            
            std::cerr << "[crew::cleanup]" << std::endl;
            member *p = static_cast<member*>(wksp);
            while(nthr>0)
            {
                member &m = p[--nthr];
                m.work.join();
                m.work.~thread();
            }
            memory::kind<memory::global>::release(wksp, wlen);
        }
        
        
        void  crew:: place(  )
        {
            scoped_lock guard(access);
            assert( size > 0 );
            const size_t  n  = hardware::nprocs();
            member       *m  = static_cast<member *>(wksp);
            //------------------------------------------------------------------
            // assign main thread
            //------------------------------------------------------------------
            std::cerr << "-- main thread: " << std::endl;
            thread::assign_cpu( thread::get_current_handle(), cpu_index(0,n) );
            
            //------------------------------------------------------------------
            // assign team thread
            //------------------------------------------------------------------
            std::cerr << "-- members    : " << std::endl;
            for( size_t i=0; i < size; ++i )
            {
                thread::assign_cpu( m[i].work.get_handle(), cpu_index(i,n) );
            }
        }
        
        
        void crew:: launch(void *args) throw()
        {
            //--------------------------------------------------------------
            // tumble to the engine
            //--------------------------------------------------------------
            assert(args);
            member     &m = *static_cast<member*>(args);
            
            assert(m.crew_p);
            m.crew_p->engine(m.rank);
        }
        
        void crew:: engine( size_t rank ) throw()
        {
            //--------------------------------------------------------------
            //
            // Entering the engine
            //
            //--------------------------------------------------------------
            context ctx(rank,size,access);
            { scoped_lock guard(access); std::cerr << "[crew::engine] start thread " << size << "." << rank << std::endl; }
            
            
            
            access.lock();
        CYCLE:
            //--------------------------------------------------------------
            //
            // CYCLE
            //
            //--------------------------------------------------------------
            
            //--------------------------------------------------------------
            // LOCKED: update ready status
            //--------------------------------------------------------------
            ++ready;
            
            //--------------------------------------------------------------
            // UNLOCK and WAIT
            //--------------------------------------------------------------
            enter.wait(access);
            
            //--------------------------------------------------------------
            // LOCKED return: test for stop
            //--------------------------------------------------------------
            if( stop )
            {
                --built;
                std::cerr << "[crew::engine] shutdown thread " << ctx.size << "." << ctx.rank << std::endl;
                access.unlock();
                return;
            }
            access.unlock();
            
            
            //--------------------------------------------------------------
            // UNLOCKED: perform task associated to this local context
            //--------------------------------------------------------------
            assert(proc);
            (*proc)(ctx);
            
            
            //--------------------------------------------------------------
            // Get a lock to check that everyone is ok
            //--------------------------------------------------------------
            access.lock();
            assert(activ>0);
            if( --activ <= 0 )
            {
                leave.signal();
            }
            goto CYCLE; // access is already locked at this point
        }
        
        
        void crew:: run( task &sub ) throw()
        {
            //------------------------------------------------------------------
            //-- get a lock on a synchronized crew
            //------------------------------------------------------------------
            for(;;)
            {
                if( access.try_lock() )
                {
                    if( ready >= size )
                    {
                        break;
                    }
                    access.unlock();
                }
            }
            
            //------------------------------------------------------------------
            //-- the access is locked: init cycle
            //------------------------------------------------------------------
            ready = 0;
            activ = size;
            
            //------------------------------------------------------------------
            //-- prepare the enter condition
            //------------------------------------------------------------------
            enter.broadcast();
            
            //------------------------------------------------------------------
            //-- prepare the task to perform
            //------------------------------------------------------------------
            proc = &sub;
            
            //------------------------------------------------------------------
            //-- wait on leave while releasing access lock !
            //------------------------------------------------------------------
            leave.wait(access);
            
            //------------------------------------------------------------------
            //-- everybody is done
            //------------------------------------------------------------------
            access.unlock();
        }
        
        
    }
}
