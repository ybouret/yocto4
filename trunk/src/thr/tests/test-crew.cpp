#include "yocto/utest/run.hpp"
#include "yocto/threading/crew.hpp"
#include "yocto/threading/window.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/ptr/shared.hpp"

using namespace yocto;

namespace {
    
    class Sum
    {
    public:
        
        const array<double> *pA, *pB;
        array<double>       *pC;
        
        inline Sum() :
        pA(0), pB(0), pC(0)
        {
        }
        
        inline ~Sum() throw()
        {
        }
        
        
        void run( threading::context &ctx )
        {
            //{ scoped_lock guard( ctx.access ); std::cerr << "Sum::run " << ctx.rank << std::endl; }
            
            assert(pA);
            assert(pB);
            assert(pC);
            
            const threading::window &w = ctx.as<threading::window>();
            const array<double> &A = *pA;
            const array<double> &B = *pB;
            array<double>       &C = *pC;
            
            const size_t i0 = w.start;
            const size_t i1 = w.final;
            for( size_t iter=1; iter <= 10; ++iter)
            {
                for( size_t i=i1; i >= i0; --i )
                {
                    C[i] = A[i] + B[i];
                }
            }
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Sum);
    };
    
}

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(crew)
{
    
    
#if 0
    size_t N=10000;
    {
        threading::crew nope;
    }
    
    threading::crew mt;
    mt.dispatch<threading::window>(N, 1);
    for(size_t i=0;i<mt.size;++i)
    {
        const threading::window &w = mt[i].as<threading::window>();
        std::cerr << "\t-- thread #" << mt.size << "." << i << " : " << w.start << " -> " << w.final << " #=" << w.count << std::endl;
    }
    
    std::cerr << "Now in main program" << std::endl;
    Sum s;
    
    threading::crew::task t( &s, & Sum::run );
    
    vector<double> A(N,0), B(N,0), C(N,0);
    s.pA = &A;
    s.pB = &B;
    s.pC = &C;
    
    
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
#endif
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(cwin)
{
#if 0
    size_t length = 1000;
    if(argc>1)
        length = strconv::to<size_t>(argv[1],"length");
    
    threading::crew team;
    for(size_t i=0;i<team.size;++i)
    {
        threading::context       &ctx = team[i];
        const threading::window   win(ctx,length,0);
        std::cerr << "win #" << i << " : " << win.start << " => " << win.final << ", #=" << win.count << std::endl;
        ctx.make<threading::window>(win);
    }
    
    team.dispatch<threading::window>(length,1);
#endif
    
}
YOCTO_UNIT_TEST_DONE()


namespace {
    class Lazy
    {
    public:
        explicit Lazy() throw() {}
        virtual ~Lazy() throw() {}
        
        
        void run( threading::context &ctx ) throw()
        {
            const double dt = ctx.as<double>();
            wtime::sleep(dt);
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lazy);
    };
}

YOCTO_UNIT_TEST_IMPL(lazy)
{
#if 0
    double sleep_total = 1.0;
    
    threading::crew team;
    const double dt = sleep_total / team.size;
    
    for(size_t i=0;i<team.size;++i)
    {
        team[i].make<double>(dt);
    }
    
    Lazy lazy;
    threading::crew::task tsk( &lazy, & Lazy::run );
    
    const size_t CYCLES = 10;
    wtime chrono;
    double ell = 0;
    for(size_t i=1; i <= CYCLES; ++i)
    {
        std::cerr << "Cycle " << i << std::endl;
        std::cerr.flush();
        chrono.start();
        team.run(tsk);
        ell += chrono.query();
    }
    std::cerr << "Average time=" << ell/CYCLES << std::endl;
#endif
}
YOCTO_UNIT_TEST_DONE()



