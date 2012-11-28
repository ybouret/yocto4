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
            guard( "crew" ),
            wlen( size * sizeof(member) ),
            wksp( memory::kind<memory::global>::acquire(wlen) ),
            n_ok(0)
            {
                initialize();
            }
            
            virtual ~crew() throw()
            {
                destroy();
            }
            
            size_t cpu_index( size_t i, size_t n ) const throw()
            {
                const size_t j =  ( root + i % size ) % n;
                std::cerr << " |_ assign on CPU #" << j << "/" << n << std::endl;
                return j;
            }
            
        private:
            mutex  guard;
            size_t wlen;
            void  *wksp;
            size_t n_ok;
            YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
            
            void initialize()
            {
                try
                {
                    //----------------------------------------------------------
                    // prepare the threads
                    //----------------------------------------------------------
                    member *p = static_cast<member*>(wksp);
                    while(n_ok<size)
                    {
                        member &m = p[n_ok];
                        //-- set member info
                        m.crew_p = this;
                        m.rank   = n_ok;
                        
                        //-- launch thread
                        new (&m.work) thread( crew::launch, &m);
                        
                        n_ok++;
                    }
                    //----------------------------------------------------------
                    // place them
                    //----------------------------------------------------------
                    place();
                    
                }
                catch(...)
                {
                    destroy();
                    throw;
                }
            }
            
            
            void destroy() throw()
            {
                member *p = static_cast<member*>(wksp);
                while(n_ok>0)
                {
                    --n_ok;
                    member &m = p[n_ok];
                    m.work.join();
                    m.work.~thread();
                }
                memory::kind<memory::global>::release(wksp, wlen);
            }
            
            
            static void launch(void *args) throw()
            {
                //--------------------------------------------------------------
                // fetch data
                //--------------------------------------------------------------
                assert(args);
                member &m = *static_cast<member*>(args); assert(m.crew_p);
                crew   &c = *(m.crew_p);
                
                //--------------------------------------------------------------
                // create context for task
                //--------------------------------------------------------------
                context ctx(m.rank,c.size,c.guard);
                {
                    scoped_lock keep( ctx.access );
                    std::cerr << "In thread " << ctx.rank << "/" << ctx.size << std::endl;
                }
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
        };
        
    }
    
}

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(crew)
{
    threading::crew simd;
}
YOCTO_UNIT_TEST_DONE()
