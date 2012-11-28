#include "yocto/utest/run.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/threading/layout.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/hw.hpp"

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
                
                context( size_t r, size_t s, lockable &lock_ref) throw() :
                rank(r),
                indx(rank+1),
                size(s),
                access(lock_ref)
                {
                }
                
                ~context() throw()
                {
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(context);
            };
            
            //! lacunar class for by-hand construction
            class member
            {
            public:
                crew         *crew_p;
                size_t        rank;
                thread        work;
            private:
            };
            
            explicit crew() :
            layout(),
            guard( "threading::crew" ),
            ready(),
            waiting(0),
            engines(0),
            stop( false ),
            wlen( size * sizeof(member) ),
            wksp( memory::kind<memory::global>::acquire(wlen) ),
            nthr(0)
            {
                initialize();
            }
            
            virtual ~crew() throw()
            {
                terminate();
            }
            
            size_t cpu_index( size_t i, size_t n ) const throw()
            {
                const size_t j =  ( root + i % size ) % n;
                std::cerr << "              |_ assign on CPU #" << j << "/" << n << std::endl;
                return j;
            }
            
        private:
            mutex     guard;
            condition ready;
            size_t    waiting;  //!< #waiting threads
            size_t    engines;  //!< #engines
            bool   stop;
            size_t wlen;
            void  *wksp;
            size_t nthr;
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            
            void initialize()
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
                        
                        engines = ++nthr;
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
            
            void terminate() throw()
            {
                std::cerr << "Terminate" << std::endl;
                stop = true;
                
                do
                {
                    ready.broadcast();
                }
                while(engines>0);
                
                member *p = static_cast<member*>(wksp);
                while(nthr>0)
                {
                    member &m = p[--nthr];
                    m.work.join();
                    m.work.~thread();
                }
                memory::kind<memory::global>::release(wksp, wlen);
            }
            
            
            void  place(  )
            {
                scoped_lock keep(guard);
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
            
            
            static void launch(void *args) throw()
            {
                //--------------------------------------------------------------
                // tumble to the engine
                //--------------------------------------------------------------
                assert(args);
                member     &m = *static_cast<member*>(args); assert(m.crew_p);
                m.crew_p->engine(m.rank);
            }
            
            
            void engine( size_t rank ) throw()
            {
                //--------------------------------------------------------------
                // Entering the engine
                //--------------------------------------------------------------
                context ctx(rank,size,guard);
                {
                    scoped_lock keep( guard );
                    std::cerr << "enter thread " << size << "." << rank << std::endl;
                }
                
                //--------------------------------------------------------------
                //
                // wait for job to do
                //
                //--------------------------------------------------------------
                guard.lock();
                ++waiting;
                std::cerr << "#waiting=" << waiting << std::endl;
                ready.wait(guard);
                --waiting;
                std::cerr << "running " << size << "." << rank << std::endl;
                
                
                if( stop )
                {
                    --engines;
                    std::cerr << "leave thread " << ctx.size << "." << ctx.rank << ", #engines=" << engines << std::endl;
                    guard.unlock();
                    return;
                }
                guard.unlock();
                
            }
            
        };
        
    }
    
}

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(crew)
{
    threading::crew simd;
    std::cerr << "Now in main program" << std::endl;
}
YOCTO_UNIT_TEST_DONE()
