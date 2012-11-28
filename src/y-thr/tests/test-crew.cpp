#include "yocto/utest/run.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/threading/layout.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/hw.hpp"
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
            
            typedef functor<void,TL1(context&)> task;
            
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
            access( "crew::access" ),
            enter(),
            leave(),
            ready(0),
            activ(0),
            built(0),
            proc(0),
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
                std::cerr << "              |_ assign on CPU #" << j << "/" << size << std::endl;
                return j;
            }
            
            void run( task &sub ) throw()
            {
                //-- get a lock on a synchronized crew
                for(;;)
                {
                    if( access.try_lock() )
                    {
                        if( ready >= size)
                        {
                            //std::cerr << "[crew::run] threads are synchronized" << std::endl;
                            break;
                        }
                        access.unlock();
                    }
                }
                
                //-- the access is locked
                ready = 0;
                activ = size;
                
                //-- prepare the enter condition
                enter.broadcast();
                
                //-- prepare the task to perform
                proc = &sub;
                
                //-- wait on leave while releasing access
                leave.wait(access);
                
                
                //-- everybody is done
                access.unlock();
            }
            
        private:
            mutex      access;
            condition  enter;
            condition  leave;
            size_t     ready;
            size_t     activ;
            size_t     built;
            task     *proc;
            bool      stop;
            size_t    wlen;
            void     *wksp;
            size_t    nthr;
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
            
            
            
#define CREW_LOCK() scoped_lock guard(access)
#define CERR_LOCK CREW_LOCK(); std::cerr
            
            void terminate() throw()
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
            
            
            void  place(  )
            {
                CREW_LOCK();
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
                member     &m = *static_cast<member*>(args);
                
                assert(m.crew_p);
                m.crew_p->engine(m.rank);
            }
            
            
            void engine( size_t rank ) throw()
            {
                //--------------------------------------------------------------
                //
                // Entering the engine
                //
                //--------------------------------------------------------------
                context ctx(rank,size,access);
                {
                    CERR_LOCK << "[crew::engine] start thread " << size << "." << rank << std::endl;
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
                //std::cerr << "[crew::engine] thread "<< size << "." << rank << ": #ready=" << ready << ", #activ=" << activ << std::endl;
                
                //--------------------------------------------------------------
                // UNLOCK and WAIT
                //--------------------------------------------------------------
                enter.wait(access);
                //{ std::cerr << "[crew] running " << size << "." << rank << std::endl; }
                
                //--------------------------------------------------------------
                // LOCKED return: test for stop
                //--------------------------------------------------------------
                if( stop )
                {
                    --built;
                    std::cerr << "[crew] leave thread " << ctx.size << "." << ctx.rank << std::endl;
                    access.unlock();
                    return;
                }
                access.unlock();
                
                
                //--------------------------------------------------------------
                // UNLOCKED: perform task associated to this local context
                //--------------------------------------------------------------
                assert(proc);
                (*proc)(ctx);
                
                access.lock();
                assert(activ>0);
                if( --activ <= 0 )
                {
                    //std::cerr << "[crew::engine] all done" << std::endl;
                    leave.signal();
                }
                goto CYCLE; // access is already locked at this point
            }
            
        };
        
    }
    
}

using namespace yocto;

#include "yocto/sequence/vector.hpp"
#include "yocto/wtime.hpp"

class Sum
{
public:
    vector<size_t>       ini;
    vector<size_t>       end;
    const array<double> *pA, *pB;
    array<double>   *pC;
    
    inline Sum() : ini(), end(), pA(0), pB(0), pC(0)
    {
        
    }
    
    inline ~Sum() throw()
    {
    }
    
    void prepare( size_t size, size_t N )
    {
        ini.make(size,0);
        end.make(size,0);
        size_t i = 1;
        for( size_t rank = 0; rank < size; ++rank )
        {
            const size_t todo = N / ( size-rank );
            ini[rank+1] = i;
            i += todo;
            end[rank+1] = i-1;
            N -= todo;
            std::cerr << "rank " << rank << ": " << ini[rank+1] << " -> " << end[rank+1] << std::endl;
        }
    }
    
    void run( threading::crew::context &ctx )
    {
        //{ scoped_lock guard( ctx.access ); std::cerr << "Sum::run " << ctx.rank << std::endl; }
        
        assert(pA);
        assert(pB);
        assert(pC);
        const array<double> &A = *pA;
        const array<double> &B = *pB;
        array<double>       &C = *pC;
        
        const size_t i0 = ini[ ctx.indx ];
        const size_t i1 = end[ ctx.indx ];
        for( size_t i=i1; i >= i0; --i )
            C[i] = A[i] + B[i];
    
    }
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Sum);
};

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(crew)
{
    
    size_t N=10000;
    
    {
        threading::crew nope;
    }
    
    threading::crew mt;
    std::cerr << "Now in main program" << std::endl;
    Sum s;
    threading::crew::task t( &s, & Sum::run );
    
    vector<double> A(N,0), B(N,0), C(N,0);
    s.pA = &A;
    s.pB = &B;
    s.pC = &C;
    s.prepare( mt.size, N );
    
    for( size_t i=1; i <= N; ++i )
    {
        A[i] = 100 * ( 0.5 - alea<double>() );
        B[i] = 100 * ( 0.5 - alea<double>() );
    }
    
    const size_t CYCLES=1000;
    wtime chrono;
    chrono.start();
    for( size_t cycle=1; cycle <= CYCLES; ++cycle )
    {
        mt.run(t);
    }
    const double ell = chrono.query();
    std::cerr << std::endl << "Speed= " << (CYCLES/ell)/1000.0 << " kcycles/s" << std::endl << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
