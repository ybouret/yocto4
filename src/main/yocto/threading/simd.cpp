#include "yocto/threading/simd.hpp"
#include "yocto/memory/global.hpp"

#include <iostream>

namespace yocto
{
    namespace threading
    {

#if 0
        void SIMD:: free() throw()
        {
            SIMD &self = *this;
            for(size_t i=1;i<=size;++i)
            {
                self[size-i].free();
            }
        }

        void SIMD:: release() throw()
        {
            SIMD &self = *this;
            for(size_t i=1;i<=size;++i)
            {
                Context &ctx = self[size-i];
                ctx.free();
                ctx.deallocate();
            }
        }
#endif

        
#define YOCTO_SIMD_CTOR() \
workers( "SIMD" ),        \
access( workers.access ), \
enter(),                  \
leave(),                  \
ready(0),                 \
activ(0),                 \
stop(false),              \
built(0),                 \
kproc(0),                 \
wlen(0),                  \
wksp(0)
        
        
        SIMD:: SIMD() :
        layout(),
        YOCTO_SIMD_CTOR()
        {
            initialize();
        }
        
        SIMD:: SIMD( size_t num_threads, size_t thread_offset ) :
        layout( num_threads, thread_offset),
        YOCTO_SIMD_CTOR()
        {
            initialize();
        }
        
        SIMD:: ~SIMD() throw()
        {
            terminate();
        }
        
        namespace
        {
            class member
            {
            public:
                SIMD         *simd;
                SIMD::Context ctx;
                
            private:
            };
        }

#if 0
        SIMD::Context & SIMD:: operator[]( const size_t rank ) throw()
        {
            assert(rank<size);
            return (static_cast<member *>(wksp)+rank)->ctx;
        }
        
        const  SIMD::Context & SIMD:: operator[]( const size_t rank ) const throw()
        {
            assert(rank<size);
            return (static_cast<member *>(wksp)+rank)->ctx;
        }
#endif
        
        void SIMD:: create_contexts()
        {
            assert(0==wlen);
            assert(0==wksp);
            
            wlen = sizeof(member) * size;
            wksp = memory::kind<memory::global>::acquire( wlen );
            
            member *members = static_cast<member *>(wksp);
            for(size_t rank=0;rank<size;++rank)
            {
                member &m = members[rank];
                m.simd    = this;
                new ( &m.ctx ) Context(rank,size,access);
            }
        }
        
        void SIMD:: delete_contexts() throw()
        {
            if(wlen>0)
            {
                assert(wksp);
                size_t rank = size;
                member *members = static_cast<member *>(wksp);
                while(rank>0)
                {
                    member &m = members[--rank];
                    m.ctx.~Context();
                }
                memory::kind<memory::global>::release(wksp, wlen);
            }
        }
        
        void SIMD:: initialize()
        {
            
            try
            {
                //______________________________________________________________
                //
                // acquire threads
                //______________________________________________________________
                workers.reserve(size);
                assert(workers.pool.size==size);
                
                //______________________________________________________________
                //
                // acquire contexts
                //______________________________________________________________
                create_contexts();
                
                //______________________________________________________________
                //
                // start engines
                //______________________________________________________________
                assert(ready==0);
                member *members = static_cast<member *>(wksp);
                for(size_t rank=0;rank<size;++rank)
                {
                    workers.launch( SIMD::subroutine, &members[rank] );
                    ++built;
                }
                
                //______________________________________________________________
                //
                // First Synchro
                //______________________________________________________________
                for(;;)
                {
                    if( access.try_lock() )
                    {
                        if(ready<size)
                        {
                            access.unlock();
                            continue;
                        }
                        access.unlock();
                        std::cerr << "[SIMD] synchronized" << std::endl;
                        break;
                    }
                }
                
                //______________________________________________________________
                //
                // placement
                //______________________________________________________________
                YOCTO_LOCK(access);
                std::cerr << "[SIMD] control" << std::endl;
                assign_current_thread_on( cpu_index_of(0));
                
                std::cerr << "[SIMD] workers" << std::endl;
                size_t iThread = 0;
                for(thread *thr = workers.head; thr; thr=thr->next)
                {
                    thr->on_cpu( cpu_index_of(iThread++) );
                }
                
                
            }
            catch(...)
            {
                terminate();
                throw;
            }
            
        }
        
        void SIMD:: terminate() throw()
        {
            //------------------------------------------------------------------
            // write the stop condition
            //------------------------------------------------------------------
            {
                YOCTO_LOCK(access);
                std::cerr << "[SIMD] terminate" << std::endl;
                stop = true;
            }
            
            //------------------------------------------------------------------
            // broadcast until everybody comes back !
            //------------------------------------------------------------------
            for(;;)
            {
                enter.broadcast();
                YOCTO_LOCK(access);
                if(built<=0) break;
            }
            
            //------------------------------------------------------------------
            // release resources
            //------------------------------------------------------------------
            workers.finish();
            delete_contexts();
        }
        
        void SIMD:: subroutine( void *args ) throw()
        {
            //-- call the C++ code directly
            assert(args);
            member  &m   = *static_cast<member *>(args);
            assert(m.simd);
            m.simd->engine( m.ctx.rank );
        }
        
        
        void SIMD:: engine(size_t rank ) throw()
        {
            //------------------------------------------------------------------
            //
            // one time data init
            //
            //------------------------------------------------------------------
            
            assert(rank<size);
            assert(wksp);
            member  &m   = *(static_cast<member *>(wksp)+rank);
            Context &ctx = m.ctx;
            assert(rank == ctx.rank );
            {
                YOCTO_LOCK(access);
                std::cerr << "[SIMD] start thread " << ctx.size << "." << ctx.rank << std::endl;
            }
            
            
            access.lock();
        CYCLE:
            //------------------------------------------------------------------
            //
            // CYCLE, start on a locked access
            //
            //------------------------------------------------------------------
            
            //--------------------------------------------------------------
            // LOCKED: update ready status
            //--------------------------------------------------------------
            ++ready;
            
            //--------------------------------------------------------------
            // UNLOCK and WAIT for a signal
            //--------------------------------------------------------------
            enter.wait(access);
            
            
            //--------------------------------------------------------------
            // LOCKED return: test for stop
            //--------------------------------------------------------------
            if( stop )
            {
                assert(built>0);
                --built;
                std::cerr << "[SIMD] shutdown thread " << ctx.size << "." << ctx.rank << std::endl;
                access.unlock();
                return;
            }
            
            //std::cerr << "[SIMD] cycle " << ctx.size << "." << ctx.rank << std::endl;
            access.unlock();
            //--------------------------------------------------------------
            // UNLOCKED: perform task associated to this local context
            //--------------------------------------------------------------
            assert(kproc);
            (*kproc)(ctx);
            
            
            //--------------------------------------------------------------
            // Get a lock to check that everyone is ok
            //--------------------------------------------------------------
            access.lock();
            assert(activ>0);
            if( --activ <= 0 )
            {
                leave.signal(); // main thread will restart
            }
            goto CYCLE; // access is already locked at this point
            
        }
        
        
        void SIMD:: operator()( Kernel &K )
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
                    // TODO: wait a little bit ?
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
            kproc = &K;
            
            //------------------------------------------------------------------
            //-- wait on leave while releasing access lock !
            //------------------------------------------------------------------
            //std::cerr << "[SIMD]: cycle..." << std::endl;
            leave.wait(access);
            
            //------------------------------------------------------------------
            //-- everybody is done for this cycle
            //------------------------------------------------------------------
            access.unlock();
            
        }
        

    }
}
