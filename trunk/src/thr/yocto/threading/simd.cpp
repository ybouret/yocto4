#include "yocto/threading/simd.hpp"
#include "yocto/memory/global.hpp"

#include <iostream>

namespace yocto
{
    namespace threading
    {
        
#define YOCTO_SIMD_CTOR() \
workers( "SIMD" ), \
access( workers.access ),\
enter(),\
leave(),\
ready(0),\
activ(0),\
stop(false),\
wlen(0),\
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
                SIMD   *simd;
                context ctx;
                
            private:
            };
        }
        
        
        
        
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
                new ( &m.ctx ) context(rank,size,access);
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
                    m.ctx.~context();
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
                }
                
                //______________________________________________________________
                //
                // Synchronize
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
                        std::cerr << "[SIMD]: synchronized" << std::endl;
                        break;
                    }
                }
                
                //______________________________________________________________
                //
                // placement
                //______________________________________________________________
                YOCTO_LOCK(access);
                std::cerr << "Main Thread@" << std::endl;
                assign_current_thread_on( cpu_index_of(0));
                
                std::cerr << "Work Thread@" << std::endl;
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
            context &ctx = m.ctx;
            assert(rank == ctx.rank );
            {
                YOCTO_LOCK(access);
                std::cerr << "[Start] #" << ctx.indx << "/" << ctx.size << std::endl;
            }
            
            
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
            
        }
        
        
    }
}
